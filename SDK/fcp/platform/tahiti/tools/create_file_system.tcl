#! /usr/bin/tclsh

#-------------------------------------------------------------------------------
# Copyright (C) 2019 Synaptics Inc.
# All rights reserved.
#
# SYNAPTICS, INC. CONFIDENTIAL AND PROPRIETARY
#
# The information contained in this source code file
# is strictly confidential and proprietary to Synaptics, Inc.
# ("Synaptics")
#
# No part of this file may be possessed, reproduced or distributed, in
# any form or by any means for any purpose, without the express written
# permission of Synaptics Inc.
#
# Except as otherwise specifically provided through an express agreement
# with Synaptics that governs the confidentiality, possession, use
# and distribution of the information contained in this file, SYNAPTICS
# PROVIDES THIS INFORMATION "AS IS" AND MAKES NO REPRESENTATIONS OR
# WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED
# WARRANTY OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE,
# TITLE OR NON-INFRINGEMENT, AND SPECIFICALLY DISCLAIMS SUCH WARRANTIES
# AND REPRESENTATIONS.  IN NO EVENT WILL SYNAPTICS BE LIABLE FOR ANY DAMAGES
# ARISING FROM THE USE OF THE INFORMATION CONTAINED IN THIS FILE.
#-------------------------------------------------------------------------------
#
# File Name: create_file_system.tcl
#
# Description: Tool to create file system image
#
#-------------------------------------------------------------------------------


namespace eval file_system {

    proc display_usage {} {
            puts "Script for creating file system\n"
            puts "Usage:"
            puts "====================================================================="
            puts "Options:"
            puts "\t -fs_create: for creating a dummy file system image with -fs_size."
            puts "\t -fs_add_files: for adding files into an existed file system image."
            puts "\t -fs_path: path of file system image. Default is ."
            puts "\t -fs_name: name of file system image. Default is chunk_sfs.bin."
            puts "\t -fs_size: size of file system image. Need to specify when -fs_create and 4K alignment. Default is 0x10000(64K)."
            puts "\t -files_align: files alloc unit size in file system image. bytes/4K alignment. Default is 4K alignment."
            puts "\t -files: list of files will be added into the file system image."
            puts "Typical usage:"
            puts "Create a dummy file system image and add some files into it:"
            puts "\t create_file_system.tcl -fs_create -fs_path path -fs_name chunk_sfs.bin -fs_size 0x10000 -files_align align -files files_list"
            puts "Add some new files into the existed file system image:"
            puts "\t create_file_system.tcl -fs_add_files -fs_path path -fs_name chunk_sfs.bin -files_align align -files files_list"
            puts "====================================================================="
    }

    proc parse_mode {mode} {
            set scr_id 0
            regexp {[^_]+_([A-Z0-9]{4})_} $mode {} {scr_id}
            return $scr_id
    }

    proc id2hex {app_id {module_id 0}} {
            set error_msg "bad app ID $app_id. app Ids should contain up to 4 uppercase letters"

            if {[::string length $app_id] > 4} {
                    error $error_msg;
            } 
            set acc 0
            set i 0
            binary scan $app_id "c*" chars

            foreach c $chars {
                    if {$c < 32 || $c > 95} {
                            error $error_msg
                    }
                    set acc [expr {$acc + pow(2,$i)*($c-32)}]
                    incr i 6
            }
            return 0x[format "%.6x%.2x" [expr int($acc)] $module_id];
    }

    proc hex2id {hex} {
            if {[regexp {^(0x)?([0-9A-Fa-f]{6})([0-9A-Fa-f]{2}) *$} $hex match prf aid mid] == 0} {
                    error "incorrect format for input $hex"
            }
            set app_id ""
            scan $aid %x num
            scan $mid %x module_id
            for {set i 0} {$i < 4} {incr i} {
                    set cur [expr {$num & 0x3F}]
                    set num [expr {$num >> 6}]
                    append app_id [format %c [expr {$cur + 32}]]
            }
            return [list $app_id]
            #return [list $app_id $module_id]
    }

    proc shift {list_name} {
            upvar 1 $list_name lst
            set val [lindex $lst 0]
            set lst [lrange $lst 1 end ]
            return $val
    }

    proc print {msg} {
            variable self_debug
            if {$self_debug > 0} {
                    puts "\t $msg"
            }
    }

    proc create_file_system {argv} {

        set argc [llength $argv]
    
        set fs_create       0
        set fs_add_files    0
        set fs_list         1
        variable self_debug 0

        set check_file_duplicate 1

        set fs_path         "."
        set fs_name         "chunk_sfs.bin"
        set fs_size         0x10000

        set input_files_id   [list]
        set input_files_name [list]

        set existed_files_id   [list]
        set existed_files_name [list]

        set img_path        "."
        set img_name        "fullFW_image_evk.bin"
        set append_fs       0

        set actual_flash_start_addr  0x400
        set files_align    "4K"
        set pre_fp         0
        set cur_fp         0
        puts "Entering file system utility"
        # -------- Parsing input arguments -----------
        if {$argc > 0} {
                set args [split [join $argv " "] { }]

                while {[llength $args] > 0}  {
                        set arg [shift args]
                        switch $arg {
                                -h {
                                        display_usage
                                        return 0
                                }
                                -append_fs {
                                        set append_fs 1
                                }
                                -img_path {
                                        set img_path [shift args]
                                }
                                -img_name {
                                        set img_name [shift args]
                                }
                                -fs_create {
                                        set fs_create 1
                                }
                                -fs_add_files {
                                        set fs_add_files 1
                                }
                                -fs_path {
                                        set fs_path [shift args]
                                }
                                -fs_name {
                                        set fs_name [shift args]
                                }
                                -fs_size {
                                        if {[llength $args] == 0} {
                                                puts stderr "ERROR: invalid command line input.\n"
                                                display_usage
                                                return 1
                                        } else {
                                                set fs_size [shift args]
                                                if {$fs_size > 0 && [expr $fs_size % 4096] != 0} {
                                                        puts stderr "ERROR: size of file system image must be 4K alignment\n"
                                                        display_usage
                                                        return 1
                                                }
                                        }
                                }
                                -files_align {
                                        set files_align [shift args]
                                        if {$files_align == "bytes" || $files_align == "4K"} {

                                        } else {
                                                puts stderr "ERROR: Invalid parameter, alignment arg need input bytes/4K\n"
                                                return 1
                                        }

                                }
                                -files {
                                        if {[llength $args] == 0} {
                                                puts stderr "ERROR: No files input\n"
                                                display_usage
                                                return 1
                                        } else {
                                                #puts "[llength $args]"
                                                while {[llength $args] > 0} {
                                                        set file_name [shift args]
                                                        set mode_name [parse_mode $file_name]
                                                        print "Input: mode_name=$mode_name, file_name=$file_name"
                                                        if {$mode_name != 0} {
                                                                set file_id [id2hex $mode_name]
                                                                lappend input_files_id $file_id;
                                                                lappend input_files_name $file_name;
                                                        } else {
                                                                break;
                                                        }
                                                }
                                        }
                                }
                                default {
                                        puts stderr "ERROR: unrecognized argument!!!\n\n"
                                        display_usage
                                        return 1
                                }
                        }
                }
        } else {
                puts stderr "ERROR: Need arguments!!!\n\n"
                display_usage
                return 1
        }

        if {$append_fs > 0} {
                puts "Appending sfs image into full image"
                exec cp $img_path/$img_name $img_path/$img_name.org
                set fp_img              [open $img_path/$img_name a+]
                set img_size            [exec wc -c < $img_path/$img_name]
                set img_size_4k_align   [expr ($img_size+4095)/4096*4096]
                set img_size_4k_align   [expr ($img_size_4k_align+$actual_flash_start_addr+4095)/4096*4096]
                set diff                [expr $img_size_4k_align - $img_size - $actual_flash_start_addr] 

                puts "img_size=$img_size, img_size_4k_align=$img_size_4k_align, diff=$diff"
                if {$diff > 0} {
                        fconfigure $fp_img -translation binary
                        for {set i 0} {$i < $diff} {incr i} {
                                puts -nonewline $fp_img [binary format c 0xFF]
                        }
                        close $fp_img

                        exec mv $img_path/$img_name $img_path/$img_name.pad
                        exec cat $img_path/$img_name.pad $fs_path/$fs_name > $img_path/$img_name
                        #exec rm $img_path/$img_name.pad
                        #exec rm $img_path/$img_name.org
                }
                return;
        }

        print "fs_path:$fs_path"
        print "fs_name:$fs_name"
        print "fs_size:$fs_size"
        for {set i 0} {$i < [llength $input_files_name]} {incr i} {
                print "input_files_name\[$i\]=[lindex $input_files_name $i]"
                print "input_files_id\[$i\]=[hex2id [lindex $input_files_id $i]]"
        }


        if {$fs_create > 0} {
                # -------- Creating dummy file_system image -----------
                puts "Creating dummy file system image"
                file delete -force $fs_path/$fs_name
                set fp [open $fs_path/$fs_name w+]
                fconfigure $fp -translation binary
                for {set i 0} {$i < [expr $fs_size/4]} {incr i} {
                        puts -nonewline $fp [binary format i 0xFFFFFFFF]
                }
                seek $fp 0 start
        } elseif {$fs_add_files > 0} {
                puts "Adding files into existed file system image"
                set fp [open $fs_path/$fs_name r+]
                fconfigure $fp -translation binary
                seek $fp 0 end
                set fs_size [tell $fp]
                seek $fp 0 start

                set blk_offset 0
                while {[eof $fp] != 1} {
                        set f_magic [read $fp 4]
                        binary scan [read $fp 4] "h8" f_asize
                        binary scan [read $fp 4] "h8" f_id
                        set f_id 0x[string reverse $f_id]
                        print "add_files f_magic=$f_magic f_asize=$f_asize f_id=[hex2id $f_id]"
                        if {$f_magic == "SFSF"} {
                                lappend existed_files_id $f_id;
                                if {$files_align == "4K"} {
                                        incr blk_offset $f_asize
                                        seek $fp 0 start
                                        seek $fp [expr $blk_offset * 4096] start
                                } elseif {$files_align == "bytes"} {
                                        seek $fp 0 start
                                        set used_bytes [expr $cur_fp + $f_asize]
                                        incr cur_fp $f_asize 
                                        seek $fp $used_bytes current

                                        if {[tell $fp] >= $fs_size} {
                                                break;
                                        } else {
                                                set cur_fp [tell $fp]
                                        }
                                }
                        } else {
                                if {$files_align == "4K"} {
                                        seek $fp 0 start
                                        seek $fp [expr $blk_offset * 4096] start
                                        break;
                                }  elseif {$files_align == "bytes"} {
                                        seek $fp 0 start
                                        break;
                                }
                        }
                }

        } else {
                puts stderr "ERROR: Wrong usage!!!\n\n"
                display_usage
                return 1
        }

        # -------- Checking -----------
        if {$check_file_duplicate > 0} {
                if {[llength $input_files_id] > 0} {
                        puts "Checking whether input files duplicated..."
                        
                        set comp_list $input_files_id
                        for {set i 0} {$i < [llength $comp_list]} {incr i} {
                                set comp_id [lindex $comp_list $i]
                                for {set j [expr $i + 1]} {$j < [llength $input_files_id]} {incr j} {
                                        set input_id [lindex $input_files_id $j]
                                        if {$comp_id == $input_id} {
                                                error "Error: Duplicated File [hex2id $input_id]\n\n"
                                                return 1
                                        }
                                }
                        }
                }
                if {[llength $existed_files_id] > 0} {
                        puts "Checking whether files existed already..."

                        set comp_list $existed_files_id
                        for {set i 0} {$i < [llength $comp_list]} {incr i} {
                                set comp_id [lindex $comp_list $i]
                                for {set j 0} {$j < [llength $input_files_id]} {incr j} {
                                        set input_id [lindex $input_files_id $j]
                                        if {$comp_id == $input_id} {
                                                puts "Error: File [hex2id $input_id] already existed.\n\n"
                                                return 1
                                        }
                                }
                        }
                }
        }

        # -------- Filling -----------
        puts "Filling files..."
        for {set i 0} {$i < [llength $input_files_name]} {incr i} {
                puts "\t file:[parse_mode [lindex $input_files_name $i]]"
                set fname   [lindex $input_files_name $i]
                set file_fp [open $fname r]
                fconfigure $file_fp -translation binary
                seek $file_fp 0 end
                set fsize [tell $file_fp]
                seek $file_fp 0 start
                set fdata [read $file_fp $fsize]
                close $file_fp

                print "file_name:$fname, file_size:$fsize"
                if {$fs_add_files > 0 && $files_align == "bytes"} {
                        #1. find avaliable space always from file start to end
                        seek $fp 0 start
                        set cur_fp [tell $fp]
                        while {[eof $fp] != 1} {
                                set f_magic [read $fp 4]
                                binary scan [read $fp 4] "h8" f_asize
                                binary scan [read $fp 4] "h8" f_id
                                set f_id 0x[string reverse $f_id]
                                seek $fp 0 start
                                if {$f_magic == "SFSF"} {
                                        set used_bytes [expr $cur_fp + $f_asize]
                                        set pre_fp $cur_fp
                                        incr cur_fp $f_asize 
                                        seek $fp $used_bytes current
                                        set cur_fp [tell $fp]
                                } else {
                                        set cur_fp $used_bytes
                                        seek $fp $cur_fp start 
                                        break;
                                }
                        }
                        if {[expr [tell $fp] + [expr [expr $fsize + 12 + 3]/4*4]] >= $fs_size} {
                                puts stderr "ERROR: no enough space for add files.\n"
                                return 1
                        }
                        puts "-found end-pre_fp:$pre_fp--cur_fp:$cur_fp---fp=[tell $fp], used_bytes $used_bytes fs_size $fs_size\n"
                }
                if {$files_align == "4K"} {
                        set fsize_4k_align [format %04x [expr [expr $fsize + 4095]/4096]]
                        print "fsize_4k_align=$fsize_4k_align"
                        set fsize_align $fsize_4k_align
                } elseif {$files_align == "bytes"} {
                        set fsize_bytes_align [format %04x [expr [expr $fsize + 12 + 3]/4*4]]
                        print "fsize_bytes_align=$fsize_bytes_align"
                        set fsize_align $fsize_bytes_align
                        #--------check block-------
                        set cur_fp [tell $fp]
                        if {$fs_add_files > 0} {
                                seek $fp [expr $pre_fp + 6] start
                                seek $fp $cur_fp start
                        }
                }

                puts -nonewline $fp SFSF
                puts -nonewline $fp [binary format i [scan $fsize_align %x]]
                puts -nonewline $fp [binary format i [lindex $input_files_id $i]]
                puts -nonewline $fp $fdata
                if {$files_align == "4K"} {
                        seek $fp [expr [expr [scan $fsize_align %x] * 4096] - 12 - $fsize] current
                } elseif {$files_align == "bytes"} {
                        set pre_fp $cur_fp
                        set cur_fp [tell $fp]
                        if {$cur_fp != [expr $pre_fp + [scan $fsize_align %x]]} {
                                seek $fp [expr $pre_fp + [scan $fsize_align %x] - $cur_fp] current
                        }
                }
        }

        if {$fs_list > 0} {
                # -------- Listing -----------
                puts "Listing file system image:"
                set blk_offset 0
                seek $fp 0 start
                set cur_fp [tell $fp]
                set i 0
                while {[eof $fp] != 1} {
                        set f_magic [read $fp 4]
                        binary scan [read $fp 4] "h8" f_asize
                        binary scan [read $fp 8] "h8 h8" id emet
                        
                        set f_asize1    [format %u 0x[string reverse $f_asize]]
                        set f_id        [string reverse $id]
                        set magic_emet  [string reverse $emet]
             
                        print "f_magic=$f_magic f_asize=$f_asize f_id=$f_id magic_emet=$magic_emet"
                        if {$f_magic == "SFSF"} {
                                if {$files_align == "bytes"} {
                                        set blk_offset [expr $cur_fp / 4096]
                                }
                                if {$magic_emet == "54454d45"} {
                                        puts "\t *ID: [hex2id $f_id], blk_off: $blk_offset, size: $f_asize1"
                                } else {
                                        puts "\t  ID: [hex2id $f_id], blk_off: $blk_offset, size: $f_asize1"
                                }

                                seek $fp 0 start
                                if {$files_align == "4K"} {
                                        incr blk_offset $f_asize1
                                        seek $fp [expr $blk_offset * 4096] start
                                } elseif {$files_align == "bytes"} {
                                        set used_bytes [expr $cur_fp + $f_asize1]
                                        incr cur_fp $f_asize1
                                        seek $fp $used_bytes current
                                        set cur_fp [tell $fp]    
                                        incr i
                                }
                        } else {
                                break;
                        }
                }
                if {$files_align == "4K"} {
                        if {$blk_offset > [expr $fs_size/4096]} {
                                error "file system space full."
                        } else {
                                puts "\nTotal Space: [expr $fs_size/4096] blocks"
                                puts "Space Left: [expr [expr $fs_size/4096] - $blk_offset] blocks"
                        }
                } elseif {$files_align == "bytes"} {
                        if {$used_bytes > $fs_size} {
                                error "file system space full."
                        } else {
                                puts "\nTotal Space: [expr $fs_size/4096] blocks $fs_size bytes"
                                puts "Space Left: [expr $fs_size - $used_bytes] bytes"
                        }       
                }

        }

        # -------- Done -----------
        puts "Done"
        close $fp
    }

    #Added a condition to call create_file_system to avoid a lot of errors when make image in sdk.
    if {$argv != ""} {
        #puts "argv=$argv"
        create_file_system $argv
    }
}
