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
# File Name: create_chunks.tcl
#
# Description: Tool to create chunks so as to be able to integrate with the M3
#              (BCM) code.
#
#-------------------------------------------------------------------------------
#
# SYNOPSIS
#
#  $ tclkitsh-8.5.9-win32.upx.exe ./create_chunks.tcl -ms ../m33/build/mcu_s.elf 
#    -m ../m33/build/mcu_ns.elf -s 0x1000 -d ../hifi3/buildxtensa/firmware/xtensa_prj/bin/Niue_Prod_Candidate_06/Debug/firmware
#
#-----------------------------------------------------------------
# Environment Variables with paths
# TODO: Check to see if we can get them from the environment variables
#-----------------------------------------------------------------
if { [info exist ::env(XTENSA_BUILD_TOOLS_ROOT)] } {
  set dsp_tools_root $::env(XTENSA_BUILD_TOOLS_ROOT)
} else {
  set dsp_tools_root null
}
if { [info exist ::env(XTENSA_SYSTEM)] } {
    # parse out from the variable
    if {$::tcl_platform(os) == "Linux"} {
        set b [split $::env(XTENSA_SYSTEM) /]
        foreach f $b {
            if { [string match "R*-linux" $f] } {
                regsub -all {(.)-linux} $f {\1} xtensa_version
                break
            }
        }
    } else {
        set b [split $::env(XTENSA_SYSTEM) \\]
        foreach f $b {
            if { [string match "R*-win32" $f] } {
                regsub -all {(.)-win32} $f {\1} xtensa_version
                break
            }
        }
    }
} else {
    # set a default one
    set xtensa_version "RG-2018.9"
}
if { [info exist ::env(CAPE2CHESSDIR)] } {
  set cape_tools_root $::env(CAPE2CHESSDIR)
} else {
  set cape_tools_root null
}
regsub -all {\\} $dsp_tools_root {/} dsp_tools_root_forward_slash
regsub -all {\\} $cape_tools_root {/} cape_tools_root_forward_slash
if {$::tcl_platform(os) == "Linux"} {
    set arm_readelf   "/opt/gcc-arm-none-eabi-7-2017-q4-major/bin/arm-none-eabi-readelf" 
    set arm_objcopy   "/opt/gcc-arm-none-eabi-7-2017-q4-major/bin/arm-none-eabi-objcopy" 
} else {
    set arm_readelf   "c\:/Program\ Files\ \(x86\)/GNU\ Tools\ ARM\ Embedded/7\ 2017-q4-major/bin/arm-none-eabi-readelf" 
    set arm_objcopy   "c\:/Program\ Files\ \(x86\)/GNU\ Tools\ ARM\ Embedded/7\ 2017-q4-major/bin/arm-none-eabi-objcopy" 
}
if {$::tcl_platform(os) == "Linux"} {
    set dsp_readelf   "${dsp_tools_root_forward_slash}/XtDevTools/install/tools/${xtensa_version}-linux/XtensaTools/bin/xt-readelf"
    set dsp_objcopy   "${dsp_tools_root_forward_slash}/XtDevTools/install/tools/${xtensa_version}-linux/XtensaTools/bin/xt-objcopy"
} else {
    set dsp_readelf   "${dsp_tools_root_forward_slash}/XtDevTools/install/tools/${xtensa_version}-win32/XtensaTools/bin/xt-readelf"
    set dsp_objcopy   "${dsp_tools_root_forward_slash}/XtDevTools/install/tools/${xtensa_version}-win32/XtensaTools/bin/xt-objcopy"
}
set cape_readelf   "${cape_tools_root_forward_slash}/win64/bin/WINbin/read_elf"
set cape_objcopy   "${cape_tools_root_forward_slash}/win64/bin/WINbin/read_elf"

#-----------------------------------------------------------------
# Constants Variables 
#-----------------------------------------------------------------
set section_names_to_parse {"\.text" "\.system_layout" "\.data" "\.reclaim" "\.second_text" "\.ext_text" "\.sm_data" "\.sm_rodata" "\.gnu\.sgstubs" "PMSRAM0" "\.rodata" "\.romtable"} 
set section_names_to_parse_cape { \
  "\.text\." \
  "\.rodata\." \
  "\.data\." \
  }

set section_names_to_dupliate_cape { \
  ".PCD" \
  ".XCD" \
  ".XDD" \
  ".XPD" \
  ".YMD" \
  }

set section_names_to_single_cape { \
  ".XS" \
  ".PCA" \
  ".PCB" \
  ".XCA" \
  ".XCB" \
  ".XDA" \
  ".XDB" \
  ".XPA" \
  ".XPB" \
  ".XPH" \
  ".YMA" \
  ".YMB" \
  ".ROA" \
  ".ROB" \
  }

set section_names_to_parse_lle { \
  "\.text\." \
  "\.rodata\." \
  "\.data\."
  }

set section_names_to_vector_lle { \
  ".YM24" \
  ".XM24" \
  }

set section_names_to_parse_hifi3 { \
  "\.text" \
  "\.rodata" \
  "\.data" \
  "\.romtable" \
  "\.stext0" \
  "\.sdata0" \
  "\.reclaim_code" \
  }

set ahb_sram_base_addr     0x00000000
set axi_sram_base_addr     0x00000000

# set align_mem              16
# set align_mem              256
set align_mem              4
set align_mem_cape			4

# set align_flash            1024
set align_flash            4096

set align_sfs              4096

# set encrypted_header_size  0x1870
set encrypted_header_size  0x0

# set secure_flash_offset    0x6d2d0
set secure_flash_offset    0x0

set flash_start_addr       0x400

set word_alignment			0x4

set align_head			0x100

set align_cape			32768
set align_lle			16384
set align_lle24			24576
set align_hifi			65536
set align_share			65536

set sec_attr				0
set sec_key_num				0
set sec_key1				0x0
set sec_key2				0x0
set sec_key3				0x0
set sec_key4				0x0

# M33		1
# CAPE_A	2
# CAPE_B	3
# CAPE		4 # combined
# LLE_A		5
# LLE_B		6
# LLE		7 # combined
# HIFI3		8

set parse_config		1

set base_offset       0x00000000
set base2_offset       0x00000000
set start_address     0x30000000
set view_debug			0

set roma        ".rodata.YM.4.ROA"
set romb        ".rodata.YM.4.ROB"
set roxpa       "^\.rodata\.XM\..*\.XPA$"
set roxpb       "^\.rodata\.XM\..*\.XPB$"




#-----------------------------------------------------------------
# Local functions
#-----------------------------------------------------------------
proc display_usage {} {
  puts "Script for creating chunks\n"
  puts "Usage:"
  puts "====="
  puts "create_chunks.tcl -ms Input_Secure_M33_elf_file \[-m Input_ARM_elf_file\] \
       \[-d Input_DSP_elf_file\] \[-t output_chunk_table_filename\] \
       \[-max Max_chunk_size_in_bytes\] \[-o output_dir\] \[-f flash_offset_to_put_chunks\] \
       \[-a Header_alignment\] \[-c cape elf file\] \[-p parsing config\] \[-b base offset\] \[-s start address\] \[-v\]\n"
  puts "\t-ms secure M33 elf file"
  puts "\t-m non-secure M33 elf file"
  puts "\t-d HiFi3 elf file"
  puts "\t-t output chunk table file"
  puts "\t-max max chunk size in bytes"
  puts "\t-o output directory"
  puts "\t-f flash offset to put chunks"
  puts "\t-a header alignment size 0x100 or 0x1000"
  puts "\t-c cape elf file"
  puts "\t-p parsing config"
  puts "\t-b base offset for cpu(based on -p option) to ahb mapping"
  puts "\t-s start address for M33"
  puts "\t-k key1,key2,key3,key3,key_num if encryption"
  puts "\t-v debug option"
  puts " -h : help"
}

proc shift {list_name} {
  upvar 1 $list_name lst
  set val [lindex $lst 0]
  set lst [lrange $lst 1 end ]
  return $val
}

proc dummy {addr} {
  return $addr
}

proc ahb_to_axiapb_address {addr} {
  global ahb_sram_base_addr axi_sram_base_addr
  set axiaddr [expr $addr - $ahb_sram_base_addr + $axi_sram_base_addr]
  return $axiaddr
}

proc store_chunk_table_entry {addr_trans_fn start_addr size_of_chunk chunk_type chunk_attr} {
  global align_flash encrypted_header_size align_sfs align_mem view_debug

  set chunk_codetype 3
  # This offset is to be filled later on while creating the full table
  set chunk_offset   0

  if {$chunk_type < 9} {
    # size to be align_mem aligned
    set chunk_size     [expr (($size_of_chunk + $align_mem - 1) / $align_mem) * $align_mem]

    # chunk_dest should be from original elf and later offset will be added for loading to target memory
    #set chunk_dest     [eval $addr_trans_fn $start_addr]
    set chunk_dest     $start_addr

    set chunk_length   $size_of_chunk
  } else {
    # this is for SFS later
    set chunk_size     [expr (($size_of_chunk + $align_sfs - 1) / $align_sfs) * $align_sfs]
    set chunk_dest     0
    set chunk_length   0
  }
  set chunk_table_entry [list $chunk_codetype $chunk_offset $chunk_size $chunk_dest $chunk_length $chunk_type $chunk_attr 0]
  if {$view_debug} {
    puts "\tchunk_table_entry = $chunk_table_entry"
  }
  return $chunk_table_entry
}

proc store_chunk_table_entry_cape {addr_trans_fn start_addr size_of_chunk chunk_type chunk_attr} {
  global align_flash encrypted_header_size align_sfs align_mem_cape view_debug

  set chunk_codetype 3
  # This offset is to be filled later on while creating the full table
  set chunk_offset   0

  if {$chunk_type < 9} {
    # size to be align_mem aligned
    set chunk_size     [expr (($size_of_chunk + $align_mem_cape - 1) / $align_mem_cape) * $align_mem_cape]

    # chunk_dest should be from original elf and later offset will be added for loading to target memory
    #set chunk_dest     [eval $addr_trans_fn $start_addr]
    set chunk_dest     $start_addr

    set chunk_length   $size_of_chunk
  } else {
    # this is for SFS later
    set chunk_size     [expr (($size_of_chunk + $align_sfs - 1) / $align_sfs) * $align_sfs]
    set chunk_dest     0
    set chunk_length   0
  }
  set chunk_table_entry [list $chunk_codetype $chunk_offset $chunk_size $chunk_dest $chunk_length $chunk_type $chunk_attr 0]
  if {$view_debug} {
    puts "\tchunk_table_entry_cape = $chunk_table_entry"
  }
  return $chunk_table_entry
}

proc store_chunk_table_entry_lle {addr_trans_fn start_addr size_of_chunk chunk_type chunk_attr} {
  global align_flash encrypted_header_size align_sfs align_mem_cape view_debug

  set chunk_codetype 3
  # This offset is to be filled later on while creating the full table
  set chunk_offset   0

  if {$chunk_type < 9} {
    # size to be align_mem aligned
    set chunk_size     [expr (($size_of_chunk + $align_mem_cape - 1) / $align_mem_cape) * $align_mem_cape]

    # chunk_dest should be from original elf and later offset will be added for loading to target memory
    #set chunk_dest     [eval $addr_trans_fn $start_addr]
    set chunk_dest     $start_addr

    set chunk_length   $size_of_chunk
  } else {
    # this is for SFS later
    set chunk_size     [expr (($size_of_chunk + $align_sfs - 1) / $align_sfs) * $align_sfs]
    set chunk_dest     0
    set chunk_length   0
  }
  set chunk_table_entry [list $chunk_codetype $chunk_offset $chunk_size $chunk_dest $chunk_length $chunk_type $chunk_attr 0]
  if {$view_debug} {
    puts "\tchunk_table_entry_lle = $chunk_table_entry"
  }
  return $chunk_table_entry
}

proc parse_out_sections_2b_loaded_cape {input_file readelf} {
  global section_names_to_parse_cape view_debug

  if {$view_debug} {
    puts "parse_out_sections_2b_loaded_cape"
  }

  # Parsing out the sections to be loaded in SRAM and storing them in the chunk table
  set lines [split [exec $readelf -S $input_file] "\n"]

  set tmp_sections_matched {}
  foreach sections_info $lines {
  foreach section_to_match $section_names_to_parse_cape {
    if [regexp "$section_to_match" $sections_info match] {
      if [regexp "$section_to_match*(\[^ \])" $sections_info line] {
        foreach element $line {
          break
        }
      }
      if [regexp "$element *\[^_ \]* *(\[0-9a-fA-F\]+) *(\[0-9a-fA-F\]+) *(\[0-9a-fA-F\]+) *" $sections_info line addr off sz] {}
        lappend tmp_sections_matched [list $element [expr 0x$addr] [expr 0x$off] [expr 0x$sz]]
        break
    }
  }
  }
  return $tmp_sections_matched
}

proc parse_out_sections_2b_loaded_lle {input_file readelf} {
  global section_names_to_parse_lle view_debug

  if {$view_debug} {
    puts "parse_out_sections_2b_loaded_lle"
  }

  # Parsing out the sections to be loaded in SRAM and storing them in the chunk table
  set lines [split [exec $readelf -S $input_file] "\n"]

  set tmp_sections_matched {}
  foreach sections_info $lines {
  foreach section_to_match $section_names_to_parse_lle {
    if [regexp "$section_to_match" $sections_info match] {
      if [regexp "$section_to_match*(\[^ \])" $sections_info line] {
        foreach element $line {
          break
        }
      }
      if [regexp "$element *\[^_ \]* *(\[0-9a-fA-F\]+) *(\[0-9a-fA-F\]+) *(\[0-9a-fA-F\]+) *" $sections_info line addr off sz] {}
        lappend tmp_sections_matched [list $element [expr 0x$addr] [expr 0x$off] [expr 0x$sz]]
        break
    }
  }
  }
  return $tmp_sections_matched
}


proc parse_out_sections_2b_loaded_in_hifi3 {input_file readelf} {
  global section_names_to_parse_hifi3 view_debug

  # Parsing out the sections to be loaded in SRAM and storing them in the chunk table
  set sections_info [exec $readelf -S $input_file]
  set tmp_sections_matched {}
  foreach section_to_match $section_names_to_parse_hifi3 {
    if [regexp " $section_to_match *\[^_ \]* *(\[0-9a-fA-F\]+) *(\[0-9a-fA-F\]+) *(\[0-9a-fA-F\]+) *" $sections_info line addr off sz] {
      lappend tmp_sections_matched [list $section_to_match [expr 0x$addr] [expr 0x$off] [expr 0x$sz]]
    }
  }
  if {$view_debug} {
    puts "return sections: $tmp_sections_matched"
  }
  return $tmp_sections_matched
}

proc parse_out_sections_2b_loaded_in_sram {input_file readelf} {
  global section_names_to_parse view_debug

  # Parsing out the sections to be loaded in SRAM and storing them in the chunk table
  set sections_info [exec $readelf -S $input_file]
  set tmp_sections_matched {}
  foreach section_to_match $section_names_to_parse {
    if [regexp " $section_to_match *\[^_ \]* *(\[0-9a-fA-F\]+) *(\[0-9a-fA-F\]+) *(\[0-9a-fA-F\]+) *" $sections_info line addr off sz] {
      lappend tmp_sections_matched [list $section_to_match [expr 0x$addr] [expr 0x$off] [expr 0x$sz]]
    }
  }
  if {$view_debug} {
    puts "return sections: $tmp_sections_matched"
  }
  return $tmp_sections_matched
}

proc sort_sections_2b_loaded_in_sram {tmp_sections_matched} {
  global view_debug
  # Sorting the sections by their addresses
  set sorted_sections {}
  while {[llength $tmp_sections_matched] > 0} {
    set min_indx 0
    set a_min  [lindex [lindex $tmp_sections_matched $min_indx] 1]
    for {set j 1} {$j < [llength $tmp_sections_matched]} {incr j} {
      set a_j [lindex [lindex $tmp_sections_matched $j] 1]
      if {$a_j < $a_min} {
        set min_indx $j
        set a_min    $a_j
      }
    }
    lappend sorted_sections [lindex $tmp_sections_matched $min_indx]
    set tmp_sections_matched [lreplace $tmp_sections_matched $min_indx $min_indx]
  }
  if {$view_debug} {
    puts "return sorted: $sorted_sections"
  }
  return $sorted_sections
}

proc separate_out_cape_chunks {input_file sorted_sections objcopy addr_trans_fn chunk_type} {
  global output_dir align_mem_cape chunk_table chk_num word_alignment view_debug  section_names_to_single_cape roma romb roxpa roxpb

  if {$view_debug} {
    puts "separate_out_cape_chunks"
  }
  set chunk_filename "chunk$chk_num.bin"

  set fc [open $input_file rb]
  
  # address
  set a_i_1 0
  # size
  set s_i_1 0
  set start_addr    0
  set size_of_chunk 0

  set chunk_attr 0
  set chunk_attr_c 0
  set chunk_attr_p 0
  set attr_change 0

  for {set i 0} {$i < [llength $sorted_sections]} {incr i} {

    if {$view_debug} {
      puts "================================================================================================"
    }
    # section
    set c_i [lindex [lindex $sorted_sections $i] 0]
    # address
    set a_i [lindex [lindex $sorted_sections $i] 1]
    # offset
    set o_i [lindex [lindex $sorted_sections $i] 2]
    # size
    set s_i [lindex [lindex $sorted_sections $i] 3]

	foreach section_to_match $section_names_to_single_cape {
	  set comparison -1
      if [regexp "$section_to_match *(\[^ \])" $c_i line] {
        foreach element $line {
          break
        }
	    set comparison 0
	  }
      if {$comparison == 0} {
        if {$view_debug} {
          puts "$c_i matches to $element\n"
        }
        set chunk_attr_c 1
          if {$i == 0} {
            set chunk_attr_p 1
          }
        break
      } else {
        set chunk_attr_c 0
      }
    }

	if {$chunk_attr == $chunk_attr_c} {
	  set attr_change 0
	} else {
	  if {$view_debug} {
	    puts "\t attr change between"
	  }
	  set attr_change 1
	}
    set chunk_attr $chunk_attr_c

    if {($c_i eq $roma) || ($c_i eq $romb)} {
      # Found ROA or ROB
      set chunk_attr  9
    }

    if [regexp $roxpa $c_i roxpa_match]  {
        # Found CAPE-A only rodata when single link (system memory)
        #puts "using  $roxpa  the match is: $roxpa_match"
        set chunk_attr  7
    }

    if [regexp $roxpb $c_i roxpb_match]  {
        # Found CAPE-B only rodata when single link (system memory)
        #puts "using  $roxpb  the match is: $roxpb_match"
        set chunk_attr  8
    }


    if {$view_debug} {
      puts "i = $i"
      puts "a_i_1: [format 0x%x $a_i_1]\ts_i_1: [format 0x%x $s_i_1]\tstart_addr: [format 0x%x $start_addr]\tsize_of_chunk: [format 0x%x $size_of_chunk]"
      puts "c_i: $c_i\ta_i: [format 0x%x $a_i]\to_i: [format 0x%x $o_i]\ts_i: [format 0x%x $s_i]"
    }

    # read a binary block from $input_file
    # -- offset in the image file, size
    # then write the binary block to $output_dir/tmp.bin

	seek $fc $o_i
    set section_data [read $fc $s_i]
    file delete -force $output_dir/tmp.bin
    set fout [open $output_dir/tmp.bin wb+]
    puts -nonewline $fout $section_data
    flush $fout
    close $fout	

	if {$i != 0} {
	  set a_p $a_i
	  set o_p $o_i
	  set s_p $s_i
	}

    if {$i == 0} {
      exec cat $output_dir/tmp.bin > $output_dir/$chunk_filename
      set start_addr    $a_i
      set size_of_chunk $s_i
    } elseif {(([expr $a_i-$a_i_1-$s_i_1] < $align_mem_cape) && ($a_i != 0)) && ($attr_change == 0) } {
      # zero padding for align_mem_cape
      for {set j 0} {$j < [expr $a_i-$a_i_1-$s_i_1]} {incr j} {
        exec printf "\\377" >> $output_dir/$chunk_filename
        # flush $fout
      }
      # adjust
      set size_of_chunk [expr $size_of_chunk + $a_i-$a_i_1-$s_i_1]
      exec cat $output_dir/tmp.bin >> $output_dir/$chunk_filename
      set size_of_chunk [expr $size_of_chunk + $s_i]

    } else {
      for {set j 0} {$j < [expr ((($a_i_1+$s_i_1+$align_mem_cape-1)/$align_mem_cape)*$align_mem_cape) - ($a_i_1+$s_i_1)]} {incr j} {
        exec printf "\\377" >> $output_dir/$chunk_filename
      }
      set size_of_chunk [expr $size_of_chunk + ((($a_i_1+$s_i_1+$align_mem_cape-1)/$align_mem_cape)*$align_mem_cape) - ($a_i_1+$s_i_1)]

      # Store the chunk table entry
      lappend chunk_table [store_chunk_table_entry_cape $addr_trans_fn $start_addr $size_of_chunk $chunk_type $chunk_attr_p]
      set chunk_attr_p $chunk_attr
      incr chk_num
      set chunk_filename "chunk$chk_num.bin"
      if {$view_debug} {
        puts "filename $chunk_filename"
      }
      exec cat $output_dir/tmp.bin > $output_dir/$chunk_filename

      set start_addr    $a_i
      set size_of_chunk $s_i
    }
    
    set a_i_1 $a_i
    set s_i_1 $s_i

    if {$view_debug} {
      puts "i($i)"
      puts "start_addr = $start_addr"
      puts "size_of_chunk = $size_of_chunk"
      puts "chunk_attr = $chunk_attr\n\n\n"
    }

#    set remaining 0
    set remaining [expr {$s_i_1 % $word_alignment}]

  }

  # Update last chunk file
  for {set j 0} {$j < [expr ((($a_i_1+$s_i_1+$align_mem_cape-1)/$align_mem_cape)*$align_mem_cape) - ($a_i_1+$s_i_1)]} {incr j} {
    exec printf "\\377" >> $output_dir/$chunk_filename
  }

  set size_of_chunk [expr $size_of_chunk + ((($a_i_1+$s_i_1+$align_mem_cape-1)/$align_mem_cape)*$align_mem_cape) - ($a_i_1+$s_i_1)]
  if {$view_debug} {
    puts "size j = $j"
    puts "size_of_chunk = $size_of_chunk\n"
  }
  
  # Store the chunk table entry
  lappend chunk_table [store_chunk_table_entry $addr_trans_fn $start_addr $size_of_chunk $chunk_type $chunk_attr]
  incr chk_num
  if {$view_debug} {
    puts "total $chk_num chunk sections"
  }
}

proc separate_out_lle_chunks {input_file sorted_sections objcopy addr_trans_fn chunk_type} {
  global output_dir align_mem_cape chunk_table chk_num word_alignment view_debug section_names_to_vector_lle

  if {$view_debug} {
    puts "separate_out_lle_chunks"
  }
  set chunk_filename "chunk$chk_num.bin"

  set fc [open $input_file rb]
  
  # address
  set a_i_1 0
  # size
  set s_i_1 0
  set start_addr    0
  set size_of_chunk 0

  set chunk_attr 0
  set chunk_attr_c 0
  set chunk_attr_p 0
  set attr_change 0

  for {set i 0} {$i < [llength $sorted_sections]} {incr i} {

    if {$view_debug} {
      puts "================================================================================================"
    }
    # section
    set c_i [lindex [lindex $sorted_sections $i] 0]
    # address
    set a_i [lindex [lindex $sorted_sections $i] 1]
    # offset
    set o_i [lindex [lindex $sorted_sections $i] 2]
    # size
    set s_i [lindex [lindex $sorted_sections $i] 3]

	foreach section_to_match $section_names_to_vector_lle {
	  set comparison -1
      if [regexp "$section_to_match" $c_i line] {
        foreach element $line {
          break
        }
	    set comparison 0
	  }
      if {$comparison == 0} {
        if {$view_debug} {
          puts "$c_i matches to $element\n"
        }
        set chunk_attr_c 2
          if {$i == 0} {
            set chunk_attr_p 2
          }
        break
      } else {
        set chunk_attr_c 0
      }
    }

	if {$chunk_attr == $chunk_attr_c} {
	  set attr_change 0
	} else {
	  if {$view_debug} {
	    puts "\t attr change between"
	  }
	  set attr_change 2
	}
    set chunk_attr $chunk_attr_c

    if {$view_debug} {
      puts "i = $i"
      puts "a_i_1: [format 0x%x $a_i_1]\ts_i_1: [format 0x%x $s_i_1]\tstart_addr: [format 0x%x $start_addr]\tsize_of_chunk: [format 0x%x $size_of_chunk]"
      puts "c_i: $c_i\ta_i: [format 0x%x $a_i]\to_i: [format 0x%x $o_i]\ts_i: [format 0x%x $s_i]"
    }

    # read a binary block from $input_file
    # -- offset in the image file, size
    # then write the binary block to $output_dir/tmp.bin

	seek $fc $o_i
    if {$chunk_attr_c == 2} {

      set section_bin [read $fc $s_i]
      file delete -force $output_dir/vec.bin
      set fvec [open $output_dir/vec.bin wb+]
      puts -nonewline $fvec $section_bin
      flush $fvec
      close $fvec
      set fvec [open $output_dir/vec.bin rb]

      file delete -force $output_dir/tmp.bin
      set fout [open $output_dir/tmp.bin wb+]
      set pad_data [format %c 0]

      for {set v 0} {$v < $s_i} {incr v 3} {
        puts -nonewline $fout $pad_data
        seek $fvec $v
        set section_data0 [read $fvec 1]
        set section_data1 [read $fvec 1]
        set section_data2 [read $fvec 1]
        puts -nonewline $fout $section_data2
        puts -nonewline $fout $section_data1
        puts -nonewline $fout $section_data0
      }
      close $fvec
      file delete -force $output_dir/vec.bin
      # set s_i [expr {$s_i / 3 * 4}]
      flush $fout

      set a_i [expr {$a_i * 4}]
      set s_i [expr {$s_i / 3 * 4}]
    } else {
      set section_data [read $fc $s_i]
      file delete -force $output_dir/tmp.bin
      set fout [open $output_dir/tmp.bin wb+]
      puts -nonewline $fout $section_data
      flush $fout
    }
    close $fout	

	if {$i != 0} {
	  set a_p $a_i
	  set o_p $o_i
	  set s_p $s_i
	}

    if {$i == 0} {
      exec cat $output_dir/tmp.bin > $output_dir/$chunk_filename
      set start_addr    $a_i
      set size_of_chunk $s_i
    } elseif {(([expr $a_i-$a_i_1-$s_i_1] < $align_mem_cape) && ($a_i != 0)) && ($attr_change == 0) } {
      # zero padding for align_mem_cape
      for {set j 0} {$j < [expr $a_i-$a_i_1-$s_i_1]} {incr j} {
        exec printf "\\377" >> $output_dir/$chunk_filename
        # flush $fout
      }
      # adjust
      set size_of_chunk [expr $size_of_chunk + $a_i-$a_i_1-$s_i_1]
      exec cat $output_dir/tmp.bin >> $output_dir/$chunk_filename
      set size_of_chunk [expr $size_of_chunk + $s_i]

    } else {
      for {set j 0} {$j < [expr ((($a_i_1+$s_i_1+$align_mem_cape-1)/$align_mem_cape)*$align_mem_cape) - ($a_i_1+$s_i_1)]} {incr j} {
        exec printf "\\377" >> $output_dir/$chunk_filename
      }
      set size_of_chunk [expr $size_of_chunk + ((($a_i_1+$s_i_1+$align_mem_cape-1)/$align_mem_cape)*$align_mem_cape) - ($a_i_1+$s_i_1)]

      # Store the chunk table entry
      lappend chunk_table [store_chunk_table_entry_lle $addr_trans_fn $start_addr $size_of_chunk $chunk_type $chunk_attr_p]
      set chunk_attr_p $chunk_attr
      incr chk_num
      set chunk_filename "chunk$chk_num.bin"
      if {$view_debug} {
        puts "filename $chunk_filename"
      }
      exec cat $output_dir/tmp.bin > $output_dir/$chunk_filename

      set start_addr    $a_i
      set size_of_chunk $s_i
    }
    
    set a_i_1 $a_i
    set s_i_1 $s_i

    if {$view_debug} {
      puts "i($i)"
      puts "start_addr = $start_addr"
      puts "size_of_chunk = $size_of_chunk"
      puts "chunk_attr = $chunk_attr\n\n\n"
    }

    set remaining [expr {$s_i_1 % $word_alignment}]

  }

  # Update last chunk file
  for {set j 0} {$j < [expr ((($a_i_1+$s_i_1+$align_mem_cape-1)/$align_mem_cape)*$align_mem_cape) - ($a_i_1+$s_i_1)]} {incr j} {
    exec printf "\\377" >> $output_dir/$chunk_filename
  }

  set size_of_chunk [expr $size_of_chunk + ((($a_i_1+$s_i_1+$align_mem_cape-1)/$align_mem_cape)*$align_mem_cape) - ($a_i_1+$s_i_1)]
  if {$view_debug} {
    puts "size j = $j"
    puts "size_of_chunk = $size_of_chunk\n"
  }
  
  # Store the chunk table entry
  lappend chunk_table [store_chunk_table_entry $addr_trans_fn $start_addr $size_of_chunk $chunk_type $chunk_attr]
  incr chk_num
  if {$view_debug} {
    puts "total $chk_num chunk sections"
  }
}

proc separate_out_chunks {input_file sorted_sections objcopy addr_trans_fn chunk_type} {
  global output_dir align_mem chunk_table chk_num word_alignment view_debug

  set chunk_filename "chunk$chk_num.bin"

  # address
  set a_i_1 0
  # size
  set s_i_1 0
  set start_addr    0
  set size_of_chunk 0

  set chunk_attr 0

  for {set i 0} {$i < [llength $sorted_sections]} {incr i} {
    # section
    set c_i [lindex [lindex $sorted_sections $i] 0]
    # address
    set a_i [lindex [lindex $sorted_sections $i] 1]
    # size
    set s_i [lindex [lindex $sorted_sections $i] 3]
    exec $objcopy -j $c_i -S -g -O binary $input_file $output_dir/tmp.bin

    if {$i == 0} {
      exec cat $output_dir/tmp.bin > $output_dir/$chunk_filename
      set start_addr    $a_i
      set size_of_chunk $s_i
    } elseif {([expr $a_i-$a_i_1-$s_i_1] < $align_mem) && ($a_i != 0)} {
      # zero padding for align_mem
      for {set j 0} {$j < [expr $a_i-$a_i_1-$s_i_1]} {incr j} {
        exec printf "\\377" >> $output_dir/$chunk_filename
      }
      # adjust
      set size_of_chunk [expr $size_of_chunk + $a_i-$a_i_1-$s_i_1]
      exec cat $output_dir/tmp.bin >> $output_dir/$chunk_filename
      set size_of_chunk [expr $size_of_chunk + $s_i]

    } else {
      for {set j 0} {$j < [expr ((($a_i_1+$s_i_1+$align_mem-1)/$align_mem)*$align_mem) - ($a_i_1+$s_i_1)]} {incr j} {
        exec printf "\\377" >> $output_dir/$chunk_filename
      }
      set size_of_chunk [expr $size_of_chunk + ((($a_i_1+$s_i_1+$align_mem-1)/$align_mem)*$align_mem) - ($a_i_1+$s_i_1)]
      # Store the chunk table entry
      lappend chunk_table [store_chunk_table_entry $addr_trans_fn $start_addr $size_of_chunk $chunk_type $chunk_attr]
  
      incr chk_num
      set chunk_filename "chunk$chk_num.bin"
      exec cat $output_dir/tmp.bin > $output_dir/$chunk_filename

      set start_addr    $a_i
      set size_of_chunk $s_i
    }
    
    set a_i_1 $a_i
    set s_i_1 $s_i

    if {$view_debug} {
      puts "\t i($i)"
      puts "\t start_addr = $start_addr"
      puts "\t size_of_chunk = $size_of_chunk"
    }

#    set remaining 0
    set remaining [expr {$s_i_1 % $word_alignment}]
  }

  set size_of_chunk [expr $size_of_chunk + ((($a_i_1+$s_i_1+$align_mem-1)/$align_mem)*$align_mem) - ($a_i_1+$s_i_1)]
  # Store the chunk table entry
  lappend chunk_table [store_chunk_table_entry $addr_trans_fn $start_addr $size_of_chunk $chunk_type $chunk_attr]
  incr chk_num
  if {$view_debug} {
    puts "total $chk_num chunk sections"
  }
}

#-----------------------------------------------------------------
# Main code
#-----------------------------------------------------------------

# -------- Defaults -----------
set max_chunk_size 0x100000
set output_chunk_table_filename "chunk_table.bin"
set output_dir "."
set fileSystem_size  0
set headerAlignment_size 0x100
set parseConfig_value 1
set baseAddress_offset 0x00000000
set base2Address_offset 0x00000000
set startAddress_value 0x00000000

set input_m_files {}
set input_d_files {}
set flash_offset_to_put_chunks $secure_flash_offset

# -------- Parsing input arguments -----------
if {$argc > 0} {
  set args [split [join $argv " "] { }]
  
  while {[llength $args] > 0}  {
    set arg [shift args]
    switch $arg {
      -h {
        display_usage
        exit 0
      }
      -ms {
        if {[llength $args] == 0} {
          puts stderr "ERROR: please put the M33 Secure Image Elf file name\n"
          display_usage
        } else {
          set input_ms_file [shift args]
        }
      }
      -m {
        if {[llength $args] == 0} {
          puts stderr "ERROR: please put the M33 Non-secure Image Elf file name\n"
          display_usage
        } else {
          lappend input_m_files [shift args]
        }
      }
      -d {
        if {[llength $args] == 0} {
          puts stderr "ERROR: please put the DSP Image Elf file name\n"
          display_usage
        } else {
          lappend input_d_files [shift args]
        }
      }
      -t {
        set output_chunk_table_filename [shift args]
      }
      -o {
        set output_dir [shift args]
      }
      -max {
        set max_chunk_size [shift args]
      }
      -f {
        set flash_offset_to_put_chunks [shift args]
      }
      -a {
        set headerAlignment_size [shift args]
      }
      -p {
        set parseConfig_value [shift args]
      }
      -b {
        set base_offset [shift args]
      }
      -b2 {
        set base2_offset [shift args]
      }
      -s {
        set startAddress_value [shift args]
      }
      -k {
        set secValues [shift args]
        set key_list [split $secValues ","]
        lassign $key_list sec_key1 sec_key2 sec_key3 sec_key4 sec_key_num
        set sec_attr 1
      }
      -v {
        set view_debug 1
      }
      default {
        puts stderr "ERROR: unrecognized argument!!!\n\n"
        display_usage
      }
    }
  }
} else {
  puts stderr "ERROR: Need arguments!!!\n\n"
  display_usage
  exit 2
}


if {$view_debug} {
  puts "view debug enabled"
}

#---------- Update variables -----------------
if {$headerAlignment_size > 0} {
  set align_head $headerAlignment_size
  if {$view_debug} {
    puts "\nheader alignment size = $headerAlignment_size\n"
  }
}

if {$parseConfig_value >= 0} {
  if {$parseConfig_value > 8} {
    if {$view_debug} {
      puts "parse config error $parseConfig_value"
    }
    exit
  }
  set parse_config $parseConfig_value
  if {$view_debug} {
    puts "\nparsing config for processor type = $parseConfig_value\n"
  }
}

if {$startAddress_value} {
  set start_address $startAddress_value
  if {$view_debug} {
    puts "\nstart_address = $startAddress_value\n"
  }
}

#---------- Error checks -----------------
if {$parse_config == 1} {
  if {![info exists input_ms_file]} {
    puts stderr "\n\nERROR: Please input the Secure M33 Elf File-name !!!\n\n."
    exit 2
  }
  if {[catch {file lstat $input_ms_file finfo}]} {
    puts stderr "ERROR: Cannot find the Input M33 Secure Elf file: $input_ms_file"
    exit 2
  }
}

if {$parse_config == 2} {
  if {![info exists input_d_files]} {
    puts stderr "\n\nERROR: Please input the CAPE+ Elf File-name !!!\n\n."
    exit 2
  }
#if {[catch {file lstat $input_d_files finfo}]} {
#  puts stderr "ERROR: Cannot find the Input CAPE+ Elf file: $input_ms_file"
#  exit 2
#}
}

foreach f $input_m_files {
  if {[catch {file lstat $f finfo}]} {
    puts stderr "ERROR: Cannot find the Input M33 Elf file: $f"
    exit 2
  }
}
foreach f $input_d_files {
  if {[catch {file lstat $f finfo}]} {
    puts stderr "ERROR: Cannot find the Input HiFi3 Elf file: $f"
    exit 2
  }
}

#---------- Initializations -----------------
set chunk_table {}
set chk_num     0

#---------- Working on Secure M33 elf file ----------------------
if {$parse_config == 1} {
  if {$view_debug} {
    puts "parsing for Secure M33"
  }
  set input_file      $input_ms_file
  set objcopy         $arm_objcopy
  set readelf         $arm_readelf
  set addr_trans_fn   dummy
  set chunk_type      $parse_config
  
  set tmp_sections_matched [parse_out_sections_2b_loaded_in_sram $input_file $readelf]
  set sorted_sections      [sort_sections_2b_loaded_in_sram $tmp_sections_matched]
  
  separate_out_chunks $input_file $sorted_sections $objcopy $addr_trans_fn $chunk_type
}
#---------- Working on Non-Secure M33 elf files ----------------------
if {$parse_config == 0} {
  if {$view_debug} {
    puts "parsing for Non-secure M33"
  }
  set input_file      $input_ms_file
  set objcopy         $arm_objcopy
  set readelf         $arm_readelf
  set addr_trans_fn   dummy
  set chunk_type      $parse_config

  set tmp_sections_matched [parse_out_sections_2b_loaded_in_sram $input_file $readelf]
  set sorted_sections      [sort_sections_2b_loaded_in_sram $tmp_sections_matched]

  separate_out_chunks $input_file $sorted_sections $objcopy $addr_trans_fn $chunk_type
}

#---------- Working on CAPE elf files ----------------------
if {($parse_config == 2) || ($parse_config == 3) || ($parse_config == 4)} {
  if {$view_debug} {
    puts "parsing for CAPE $parse_config"
  }
foreach d_file $input_d_files {
  set input_file      $d_file
#  set objcopy         $cape_objcopy
#  set objcopy         "objcopy"
  set objcopy         $arm_objcopy
#  set readelf         $cape_readelf
  set readelf         $arm_readelf
  set addr_trans_fn   dummy
  set chunk_type      $parse_config

  if {$view_debug} {
    puts "parse and load\n"
  }
  set tmp_sections_matched [parse_out_sections_2b_loaded_cape $input_file $readelf]
  set sorted_sections      [sort_sections_2b_loaded_in_sram $tmp_sections_matched]

  if {$view_debug} {
    puts "separate out cape chunks\n"
  }
  separate_out_cape_chunks $input_file $sorted_sections $objcopy $addr_trans_fn $chunk_type
}
}

#---------- Working on LLE elf files ----------------------
if {($parse_config == 5) || ($parse_config == 6) || ($parse_config == 7)} {
  if {$view_debug} {
    puts "parsing for LLE $parse_config"
  }
foreach d_file $input_d_files {
  set input_file      $d_file
#  set objcopy         $cape_objcopy
#  set objcopy         "objcopy"
  set objcopy         $arm_objcopy
#  set readelf         $cape_readelf
  set readelf         $arm_readelf
  set addr_trans_fn   dummy
  set chunk_type      $parse_config

  set sorted_sections [parse_out_sections_2b_loaded_lle $input_file $readelf]

  separate_out_lle_chunks $input_file $sorted_sections $objcopy $addr_trans_fn $chunk_type
}
}

#---------- Working on HiFi3 elf files ----------------------
if {$parse_config == 8} {
  if {$view_debug} {
    puts "parsing for HiFi3"
  }
foreach d_file $input_d_files {
  set input_file      $d_file
  set objcopy         $dsp_objcopy
  set readelf         $dsp_readelf
  set addr_trans_fn   dummy
  set chunk_type      $parse_config

  set tmp_sections_matched [parse_out_sections_2b_loaded_in_hifi3 $input_file $readelf]
  set sorted_sections      [sort_sections_2b_loaded_in_sram $tmp_sections_matched]

  separate_out_chunks $input_file $sorted_sections $objcopy $addr_trans_fn $chunk_type
}
}

#---------- Adding SFS information into the chunk table ----
#if {$fileSystem_size > 0} {
#  set start_addr        0
#  set size_of_chunk     $fileSystem_size
#  set addr_trans_fn     dummy
#  set chunk_type        9
#  puts "store the chunk table entry================================================SFS"
#  lappend chunk_table [store_chunk_table_entry $addr_trans_fn $start_addr $size_of_chunk $chunk_type $chunk_attr]
#}

#---------- Building the chunk table file ------------------
file delete -force $output_dir/$output_chunk_table_filename
set fp [open $output_dir/$output_chunk_table_filename w]
fconfigure $fp -translation binary

# Writing Table Header
set magic_number   0x474D4945
set table_version  0x1
set num_chunks     [expr [llength $chunk_table]-1]
set table_size     [expr ((($num_chunks+1)*8*4) + (8+6)*4)]
set chk_tabl_header [list $magic_number $table_version $align_head $parse_config $base_offset $base2_offset $table_size [expr $num_chunks + 1] $sec_attr $sec_key_num $sec_key1 $sec_key2 $sec_key3 $sec_key4]
set out_bin_data    [binary format i14 $chk_tabl_header]
puts -nonewline $fp $out_bin_data
if {$view_debug} {
  puts "\nWriting Table Header"
  puts "chk_tabl_header $chk_tabl_header"
}

# Calculating the start of the first chunk section in flash
set chunk_info [lindex $chunk_table 0]
set new_chunk_offset 0
lassign $chunk_info chunk_codetype chunk_offset chunk_size chunk_dest chunk_length chunk_type chunk_attr res2

# set chunk_offset [expr $flash_offset_to_put_chunks + $chunk_size]
# set encryp_chunk_table_size [expr ((($table_size+$encrypted_header_size+$align_flash-1)/$align_flash)*$align_flash)]
# set chunk_offset [expr $chunk_offset + $encryp_chunk_table_size]

# in real usages, offset starts from align + chunk_offset from first chunk

if {$view_debug} {
  puts "num_chunks = [expr $num_chunks + 1]"
}
#writing the chunk table
for {set i 0} {$i <= $num_chunks} {incr i} {
  set chunk_info [lindex $chunk_table $i]
  lassign $chunk_info chunk_codetype chunk_offset chunk_size chunk_dest chunk_length chunk_type chunk_attr res2
  if {$view_debug} {
    puts "\tchunk_codetype chunk_offset chunk_size chunk_dest chunk_length chunk_type chunk_attr res2"
    puts "\tchunk_info($i) = $chunk_info"
  }

  # only for SFS type - not yet for Tahiti
  if {$chunk_type == 8} {
    # Align to the 4Kbyte first before adjusting actual flash start addr
    puts "chunk_offset [expr (($chunk_offset+$align_sfs-1)/$align_sfs)*$align_sfs]"
    set chunk_offset [expr (($chunk_offset+$align_sfs-1)/$align_sfs)*$align_sfs]
    # re-align after adjusting flash atart addr
    puts "chunk_offset [expr (($chunk_offset+$flash_start_addr+$align_sfs-1)/$align_sfs)*$align_sfs]"
    set chunk_offset [expr (($chunk_offset+$flash_start_addr+$align_sfs-1)/$align_sfs)*$align_sfs]
  }

  set chunk_offset $new_chunk_offset
  set chk_tabl_entry  [list $chunk_codetype $chunk_offset $chunk_size $chunk_dest $chunk_length $chunk_type $chunk_attr $res2]
  if {$view_debug} {
    puts "\tchunk_codetype chunk_offset chunk_size chunk_dest chunk_length chunk_type chunk_attr res2"
    puts "\tchk_tabl_entry ($i) = $chk_tabl_entry "
  }
  set out_bin_data    [binary format i8 $chk_tabl_entry]
  puts -nonewline $fp $out_bin_data

  set new_chunk_offset [expr $chunk_offset + $chunk_size]
  if {$view_debug} {
    puts "new chunk_offset = $new_chunk_offset"
  }

}

#
# adding chunk#.bin files to a single chunk.bin
# get total chunk numbers
# create new file
# get a file with a number up to total
# append a file up to total number of files
# still need chunk table for parsing
# 

# create a file for appending
# each chunk section to a single file
file delete -force $output_dir/chunk.bin
set fo [open $output_dir/chunk.bin a]
for {set i 0} {$i <= $num_chunks} {incr i} {
  set chunk_filename "chunk$i.bin"
  exec cat $output_dir/$chunk_filename >> $output_dir/chunk.bin
  if {$view_debug} {
    puts "add chunk$i.bin to chunk.bin"
  }
}
close $fo

close $fp

# note: in this scheme, one chunk table can be used for parsing all of binary files
# note: the chunk table already have total number of chunk sections and size of the table, and type of each chunk

# combine (table + header_alignment)  + chunk(full sections) + padding

file delete -force $output_dir/chunk_type$chunk_type.bin
set fo [open $output_dir/chunk_type$chunk_type.bin ab]
# add header
exec cat $output_dir/$output_chunk_table_filename >> $output_dir/chunk_type$chunk_type.bin
set file_header_size [file size $output_dir/$output_chunk_table_filename]
if {$align_head < $file_header_size} {
  puts "error in header size = $file_header_size > $align_head\n"
  exit
}

set align_size [expr ($align_head - $file_header_size)]
set pad_data   0xff
for {set j 0} {$j < $align_size} {incr j} {
  puts -nonewline $fo [binary format c [expr {$pad_data}]]
}
flush $fo

# add chunk.bin with padding
exec cat $output_dir/chunk.bin >> $output_dir/chunk_type$chunk_type.bin
set file_header_size [file size $output_dir/chunk_type$chunk_type.bin]
set align_size [expr ($align_flash - (($file_header_size) % $align_flash))]
for {set j 0} {$j < $align_size} {incr j} {
  puts -nonewline $fo [binary format c [expr {$pad_data}]]
}
flush $fo

close $fo

file delete -force $output_dir/tmp.bin


