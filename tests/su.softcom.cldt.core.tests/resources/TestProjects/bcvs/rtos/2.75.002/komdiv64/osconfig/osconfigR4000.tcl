#!/usr/bin/wish -f
#
#
#
#
set dir_name [pwd]
set dir_filter "*.c"
set dir_only 0
set one_file 0
set q "\""
set thl 0
set hline 0
set current_line 0
set x_enabled 0
set current_val {}
set labelwidth 40
set plabelwidth 0
set filenames {}
set savestate   normal
set configfile "$env(HOME)/.oc2000_config"
set tunefile   "$env(HOME)/.config_tune"


if { [array names env HELP_BROWSER]=="" } {
        set HELP_BROWSER "gnome-moz-remote"
} else {
        set HELP_BROWSER $env(HELP_BROWSER)
}
#=======================================================
# Interface element creation   functions
#========================================================
proc addhlp {varname w} {
        set url [geturl $varname]
        if { $url != {} } {
          button $w.h -image $::img1 -command "show_help_bypath [getpath $varname]"
          pack $w.h -side right -anchor e
        }
}
proc getwin {varname} {
    global varnames winnames
        return [lindex $winnames [lsearch $varnames $varname]]
}
proc addmsg {varname w bind} {
#       set bal [tixBalloon $w.bln -statusbar $::status -state status -initwait 0]
#        $bal bind $bind -msg {[getemsg $varname]}
        bind $bind <Enter> "setemsg $varname Enter"
   bind $bind <Leave> "setemsg $varname Leave"
   global varnames winnames
   lappend varnames $varname
   lappend winnames $bind
}

proc create_bool { w source_line varname sc cn columns} {
   global $varname 
   set f $w.f$source_line
   frame $f 
   set text [getname $varname]
   catch {
       set ::$varname [ expr [ set ::$varname ] + 0 ]
   }
   checkbutton $f.c -text "$text" -variable $varname -command "bool_validate $varname $sc $f.c"
   if { $columns > 1 } {
           set c [expr ($cn-1)%$columns]
           set r [expr ($cn-1)/$columns]
           grid $f -column $c  -row $r -sticky we
   } else {
           pack $f -pady 1 -fill x -anchor w
   }
   pack $f.c -side left -anchor w
   addmsg $varname $f $f.c
   add_column $cn $text
   addhlp $varname $f
}
proc create_string { w source_line cn varname sc } {
   set f $w.f$source_line
   frame $f 
   set text [getname $varname]
   set le [tixLabelEntry $f.le -label $text]
   $le subwidget label configure -width $::labelwidth -anchor w -wraplength $::plabelwidth -justify left
   $le subwidget entry configure -width 40 
   $le subwidget entry configure -validate all -invcmd "mybell $varname %W" -vcmd "global_validation %P %V $varname $sc %W"
   if [info exists ::$varname] {
      $le subwidget entry insert 0 [set ::$varname]
   }
   pack $le -anchor w -side left
   pack $f -pady 1 -fill x -anchor w
   add_column $cn $text
   addmsg $varname $f $le
   addhlp $varname $f
}
proc create_int { w source_line cn varname sc width} {
   global $varname
   set f $w.f$source_line
   frame $f 
   set text [getname $varname]
   set le [tixLabelEntry $f.le -label $text]
   $le subwidget label configure -width $::labelwidth -anchor w -wraplength $::plabelwidth -justify left
   $le subwidget entry configure -width $width
   $le subwidget entry configure -validate all -invcmd "mybell $varname %W" -vcmd "global_validation %P %V $varname $sc %W"
   catch {
      $le subwidget entry insert 0 [set ::$varname]
   }
   pack $le -anchor w -side left
   pack $f -pady 1 -fill x -anchor w
   add_column $cn $text
   addmsg $varname $f $le
   addhlp $varname $f
}
proc create_menu {w source_line cn varname sc editable } {
    global $varname
    set f $w.f$source_line
    frame $f 
    set text [getname $varname]
    set le [tixComboBox $f.le -label $text -editable $editable ]
    $le configure -command "ref_validate $le $varname $sc "
    foreach v [getopt $varname] {
            $le appendhistory $v
    }
    $le subwidget label configure -width $::labelwidth -anchor w -wraplength $::plabelwidth -justify left
    $le subwidget entry configure -width 40 
    $le subwidget entry configure -validate all -invcmd "mybell $varname %W" -vcmd "global_validation %P %V $varname $sc %W"
    catch {
            $le configure -value [set ::$varname]
    }
    pack $le -anchor w -side left
    pack $f -pady 1 -fill x -anchor w
    add_column $cn $text
    addmsg $varname $f $le
    addhlp $varname $f
    return $le
}
proc create_flags {w source_line cn varname sc editable } {
    global $varname
    set f $w.f$source_line
    frame $f 
    set text [getname $varname]
    set le [tixComboBoxA $f.le -label $text -editable $editable ]
    $le configure -command "ref_validate $le $varname $sc "
    foreach v [getopt $varname] {
            $le appendhistory $v
    }
    $le subwidget label configure -width $::labelwidth -anchor w -wraplength $::plabelwidth -justify left
    $le subwidget entry configure -width 40 
    $le subwidget entry configure -validate all -invcmd "mybell $varname %W" -vcmd "global_validation %P %V $varname $sc %W"
    catch {
            $le configure -value [set ::$varname]
    }
    pack $le -anchor w -side left
    pack $f -pady 1 -fill x -anchor w
    add_column $cn $text
    addmsg $varname $f $le
    addhlp $varname $f
    return $le
}
proc create_fdialog { w source_line cn varname add sc } {
        global $varname
    set f $w.f$source_line
    frame $f 
    set text [getname $varname]
    set le [tixLabelEntry $f.le$source_line -label $text]
    $le subwidget label configure -width $::labelwidth -anchor w -wraplength $::plabelwidth -justify left
    $le subwidget entry configure -width 40 
    $le subwidget entry configure -validate all -invcmd "mybell $varname %W" -vcmd "global_validation %P %V $varname $sc %W"
    catch {
        $le subwidget entry insert 0 [set ::$varname]
    }
    button $f.b -image $::img2 -command "file_dialog [$le subwidget entry] $add"
    pack $le $f.b -anchor w -side left
    addhlp $varname $f
    pack $f -pady 1 -expand on -anchor w -fill x
    addmsg $varname $f $le
    add_column $cn $text
}
proc create_ldialog { w source_line cn varname list sc } {
        global $varname
    set f $w.f$source_line
    frame $f 
    set text [getname $varname]
    set le [tixLabelEntry $f.le$source_line -label $text]
    $le subwidget label configure -width $::labelwidth -anchor w -wraplength $::plabelwidth -justify left
    $le subwidget entry configure -width 40 
    $le subwidget entry configure -validate all -invcmd "mybell $varname %W" -vcmd "global_validation %P %V $varname $sc %W"
    $le subwidget entry insert 0 [set ::$varname]
    button $f.b -image $::img2 -command "list_dialog [$le subwidget entry] $list"
    pack $le $f.b -anchor w -side left
    addhlp $varname $f
    pack $f -pady 1 -expand on -anchor w -fill x
    addmsg $varname $f $le
    add_column $cn $text
}

proc tree_config { cmd varname } {
    global h
    global img1 img3
    if { [set ::$varname] == 1 } {
        $h entryconfigure $cmd -image $img1 -state enabled
    } else {
        $h entryconfigure $cmd -image $img3 -state disabled
    }
}
proc add_column { cn text } {
    global thl
    if { "$thl" != "0" } {
       $thl header create $cn  -text $text -style $::style
       $thl column width $cn $::colwidth
    }
}
proc add_checkbutton {w1 str varname w2} {
   checkbutton $w1.m -text $str -variable $varname  -command "bool_validate $varname -1 $w2"
   addmsg $varname $w1 $w1.m
}
proc bc00 { w l } {
    set a [winfo children $w]
    foreach i $a {
       switch [winfo class $i] {
           Frame "bc0 $i $l"
           TixLabelEntry { 
                   set a [$i subwidget entry]
                   set b [lindex [$a cget -vcmd] 3]$l
                   global $b
                   set v [$a cget -validate]
                   $a configure -validate none 
                   set state [$a configure -state]
                   $a configure -state normal
                   $a delete 0 end
                   $a insert 0 [set $b]
                   $a configure -state [lindex $state 4]
                   after idle "$a configure -validate $v"
           }
           TixComboBoxA -
           TixComboBox { 
                   set a [$i subwidget entry]
                   set b [lindex [$a cget -vcmd] 3]$l
                   global $b
                   $a configure -validate none 
                   $i configure -value [set $b]
                   after idle "$a configure -validate all"
           }
           Checkbutton {
                set b [$i cget -variable]
                set ::$b [set ::$b$l]
           }
       }
    }
}
proc bc0 { wq l } {
    catch { 
    foreach varname $::arrvars {
                set w [getwin $varname]
        switch [winfo class $w] {
           TixLabelEntry { 
                   set a [$w subwidget entry]
                   set state [$a configure -state]
                   $a configure -state normal
                   $a delete 0 end
                   $a insert 0 [set ::$varname$l]
                   $a configure -state [lindex $state 4]
                        }
           TixComboBoxA -
           TixComboBox { 
                   $w configure -value [set ::$varname$l]
           }
            Checkbutton {
                set ::$varname [set ::$varname$l]
           }
                }

        }
    }
}
proc bc { w l } {
    after idle "set ::current_line $l; bc0 $w $l"
}

proc create_dialog {w ncols nlines varname type line inline columns} {
   global thl
   set w2 $w.w$line
   if {$inline==1} {
        frame $w2 -relief sunken -borderwidth 2
        set w1 $w2
   } else {
        tixScrolledWindow $w2
        set w1 [$w2 subwidget window]
        global varnames winnames
        set varnames {}
        set winnames {}
        
   }
   set str [getname $varname]
   if { $str != "" } {
        switch $type {
                0 "message $w1.m -text $str -width 200 -justify center"
                1 "add_checkbutton $w1 $str $varname $w1.top"
        }
        pack $w1.m -side top 
   }
   update
   if { $nlines > 1 } {
        set ::colwidth  [expr ([winfo width .top.f]-60)/($ncols-1)]
        set ::style [tixDisplayStyle text -wraplength $::colwidth -justify center]        
        set thls [tixSHList $w1.list -columns $ncols -height [expr ($nlines+1)*24]]
        pack $thls  -fill both -expand 1
        set thl [$thls subwidget hlist]
        $thl configure -header 1 -browsecmd "bc $w2"
   } else {
        set thl 0
   }
#============= Controls frame ===========================
# contains all other controls of dialog
   frame $w1.top  -relief raised
   pack $w1.top -side top -fill x
   pack $w2 -fill both -expand 1 

   for {set i 0} { $i < $columns } { incr i } {
        grid columnconfigure $w1.top $i -weight 2
   }
   after idle screen_setstate
   return $w1
}


set oldpath 0
proc browse0 {path} {
    catch {
    global h w oldpath
    set oldpath $path
    if { [winfo exists $w.top.f.f] } then { destroy $w.top.f.f }
    frame $w.top.f.f
    set a [$h info data $path]
    if { $a != "" } {
    $a $w.top.f.f
    }
    pack $w.top.f.f -side left -fill both -expand yes
    }
}
proc browse {path} {
    global h w oldpath
    if { $path != $oldpath } {
        after idle "browse0 $path"
    }
}
proc x_startup {title} {
loadlocale
if { [lsearch -exact [package names] Tk] == -1 } return
set ::x_enabled 1
## package require Tix
tix_setup
#============ NEW =================
option add *Checkbutton.selectColor red
if { [file exists $::tunefile] } {
option readfile  $::tunefile
} else {
option add *font {courier 8}
}
# wm title . $title
wm title . [pwd]

global w tree h img1 img2 img3
set w ""
set ::leftsize 300
if { [file exists $::configfile] } {
        set f [open $::configfile r]
        gets $f line
        gets $f ::leftsize
        close $f
        wm geometry . =$line
} else {
        wm geometry $w. =790x550
}
#=============  Status line ======================================
   frame $w.s -borderwidth 2 -relief sunken
   set ::status [label $w.s.status -anchor w]
   pack $::status  -fill x 
   pack $w.s -fill x -side bottom
   # -side bottom
   set ::plabelwidth [expr [font measure [$w.s.status cget -font] "Ш"]*$::labelwidth-15]
#============= Bottom frame ===========================
   frame $w.buttons -relief raised -borderwidth 2
   pack $w.buttons -side bottom -fill x 
if 0 {   
   tixComboBox $w.buttons.lb -command "setproject" -state disabled
   $w.buttons.lb   subwidget entry configure -width 30
   menubutton $w.buttons.proj
   set m [menu $w.buttons.proj.m -tearoff 0]
   $m add command -label [lcget {Load}] -command "project_button_load $w.buttons.lb"
   $m add command -label [lcget {Change}] -command "project_button_change $w.buttons.lb" -state disabled
   $m add command -label [lcget {Save}] -command "project_button_save $w.buttons.lb" -state disabled

   $w.buttons.proj configure -text [lcget "Project"] -underline 0 -direction below -menu $m -relief raised 
}
   button $w.buttons.save -text [lcget "Save"] -command "save $w.buttons.save" 
   button $w.buttons.saveall -text [lcget "Save All"] -command "save_all $w.buttons.saveall" -state disabled
   button $w.buttons.show -text [lcget "Show List"] -command "show_all $w.buttons.show"  -state disabled
   button $w.buttons.help -text [lcget "Help"] -command "show_help global" 
   button $w.buttons.cancel -text [lcget "Exit"] -command "qq_exit"
   button $w.buttons.se -text [lcget "Save & Exit"] -command "save $w.buttons.save;qq_exit" 
if 0 {
   pack $w.buttons.lb $w.buttons.proj $w.buttons.show $w.buttons.save $w.buttons.saveall $w.buttons.help $w.buttons.cancel -side left -expand 1
} else {
   pack  $w.buttons.save $w.buttons.help $w.buttons.cancel $w.buttons.se -side left -expand 1
}
#============= Top frame ===========================
   tixPanedWindow $w.top -orientation horizontal -panebd 3 -panerelief sunken
   pack $w.top -side top -fill both -expand yes
   $w.top add left  
   $w.top add f
#
#    set img1 [image create photo -file "bitmaps/1.gif" -height 16 -width 16]
#    set img2 [image create photo -file "bitmaps/2.gif" -height 16 -width 16]
    set img2 [tix getimage folder]
    set img1 [tix getimage textfile]
    set img3 [tix getimage minusarm]
    set ::ck_list [list [tix getimage ck_off] [tix getimage ck_on]]
    set tree [tixTree $w.top.left.t -browsecmd browse -width $::leftsize ]
    set h [$tree subwidget hlist]
    $h configure -selectforeground black
#    pack $tree -side left -fill both 
     pack $tree -fill both -expand yes
#== right frame ================================
#    frame $w.top.right.f -width 500 -relief raised -borderwidth 2
#    pack $w.top.right.f -fill both -expand 1 
    bind . <Key-F1> "show_help local"

}
proc show_help_bypath {path} {
   set a [split $path .]
   set urlbase {}
   set url {}
   catch {
       for {set i [expr [llength $a]-1]} {$i>=0} { incr i -1 } {
            set varname [lindex $a $i]
            set urlbase [geturlbase $varname]
            if { $urlbase != {} } {
                    break
            }
       }
       set urlbase [eval concat $urlbase]
       for {set i [expr [llength $a]-1]} {$i>=0} { incr i -1 } {
            set varname [lindex $a $i]
            set url [geturl $varname]
            if { $url != {} } {
                    # puts "exec $::HELP_BROWSER file://${urlbase}/${url} >/dev/null &"
                    if { [catch  {
                    eval exec $::HELP_BROWSER file://$urlbase/${url} >/dev/null &
                    } ]
                    } {
                    puts "ERROR exec $::HELP_BROWSER file://$urlbase/${url} >/dev/null &"
                    puts "Maybe you need: HELP_BROWSER=firefox make xconfig"
                    }
                    break
            }
       }
   }
}
set curvar ""
proc show_help {type} {
        switch $type {
            global {
                   # left frame
                   show_help_bypath $::oldpath 
            }
            local {
                set w [focus]
                if { [string first .t. $w] > 0 } {
                   # left frame
                   show_help_bypath $::oldpath 
                } 
                if { [string first .f. $w] > 0 } {
                # right frame
                   show_help_bypath [getpath $::curvar].$::curvar 
                } 
            }
        }
}
proc qq_exit {} {
    set f [open $::configfile w]
    puts $f [wm geometry .]
    puts $f [winfo width $::tree]
    close $f
    exit 0
}
#/////////////////////////////////////////////////////////////////
#/////////////////////////////////////////////////////////////////
#/////////////////////////////////////////////////////////////////
#/////////////////////////////////////////////////////////////////
#/////////////////////////////////////////////////////////////////
proc load_version {argv0} {
    set base [file dirname $argv0]
    foreach filename {"../include/private/version.h" "../oc-include/private/version.h" "../include/version.h"}  {
        set filename "$base/$filename"
        if [file exists $filename] {
           set file  [open "$filename" r]
           while { [gets $file line] >= 0} {
               set a [lindex $line 1] 
               if { "$a" == "OS_MAJOR" } {
                   set ::OS_MAJOR [lindex $line 2]
               }
               if { "$a" == "OS_MINOR" } {
                   set ::OS_MINOR [lindex $line 2]
               }
               if { "$a" == "OS_RELEASE" } {
                   set ::OS_RELEASE [lindex $line 2]
               }
           }
           close $file
           puts "version  $filename  $::OS_MAJOR $::OS_MINOR $::OS_RELEASE"
           break
        }
    }
    set ::BSP_MAJOR  0
    set ::BSP_MINOR  0
    set ::BSP_BETA   0
    set filename "$base/../bsp/$::env(BSP)/include/bspversion.h"
    if [file exists $filename] {
       set file  [open "$filename" r]
       while { [gets $file line] >= 0} {
           set a [lindex $line 1] 
           if { "$a" == "BSP_VERSION_MAJOR" } {
               set ::BSP_MAJOR [lindex $line 2]
           }
           if { "$a" == "BSP_VERSION_MINOR" } {
               set ::BSP_MINOR [lindex $line 2]
           }
           if { "$a" == "BSP_VERSION_BETA" } {
               set ::BSP_BETA [lindex $line 2]
           }
       }
       close $file
       puts "bsp version  $filename  $::BSP_MAJOR $::BSP_MINOR $::BSP_BETA"
    }
}
load_version $argv0
proc tix_setup {} {
package require Tix
tixWidgetClass tixSHList {
    -classname TixSHList
    -superclass tixScrolledWidget
    -method {
    }
    -flag {
	 -columns
    }
    -configspec {
        { -columns columns Columns 1 }
    }
    -default {
	{.scrollbar			auto}
    }
}
tixWidgetClass tixComboBoxA {
    -classname TixComboBoxA
    -superclass tixLabelWidget
    -method {
	addhistory align appendhistory flash invoke insert pick popdown
    }
    -flag {
	-anchor -arrowbitmap -browsecmd -command -crossbitmap
	-disablecallback -disabledforeground -dropdown -editable
	-grab -listcmd
	-listwidth -selectmode -state
	-tickbitmap -validatecmd -value -variable
    }
    -static {
	-dropdown -fancy
    }
    -forcecall {
	-variable -selectmode -state
    }
    -configspec {
	{-arrowbitmap arrowBitmap ArrowBitmap ""}
	{-anchor anchor Anchor w}
	{-browsecmd browseCmd BrowseCmd ""}
        {-command command Command ""}
	{-crossbitmap crossBitmap CrossBitmap ""}
	{-disablecallback disableCallback DisableCallback 0 tixVerifyBoolean}
	{-disabledforeground disabledForeground DisabledForeground #606060}
	{-dropdown dropDown DropDown true tixVerifyBoolean}
	{-editable editable Editable false tixVerifyBoolean}
	{-fancy fancy Fancy false tixVerifyBoolean}
	{-grab grab Grab global}
	{-listcmd listCmd ListCmd ""}
	{-listwidth listWidth ListWidth ""}
	{-selectmode selectMode SelectMode browse}
        {-state state State normal}
	{-validatecmd validateCmd ValidateCmd ""}
	{-value value Value ""}
	{-variable variable Variable ""}
	{-tickbitmap tickBitmap TickBitmap ""}
    }
    -default {
	{*Entry.relief				sunken}
	{*TixScrolledListBox.scrollbar		auto}
	{*Listbox.exportSelection		false}
	{*Listbox.takeFocus			false}
	{*shell.borderWidth			2}
	{*shell.relief				raised}
	{*shell.cursor				arrow}
	{*Button.anchor				c}
	{*Button.borderWidth			1}
	{*Button.highlightThickness		0}
	{*Button.padX				0}
	{*Button.padY				0}
	{*tick.width				18}
	{*tick.height				18}
	{*cross.width				18}
	{*cross.height				18}
	{*arrow.anchor				c}
	{*arrow.width				15}
	{*arrow.height				18}
    }
}
}
proc tixSHList:ConstructWidget {w} {
    upvar #0 $w data

    tixChainMethod $w ConstructWidget
    set data(pw:f1) \
	[frame $w.f1 -takefocus 0 -borderwidth 1 -relief sunken ]
    set data(w:hlist) \
	[tixHList $w.f1.hlist -columns $data(-columns) -bd 0 -takefocus 1 -highlightthickness 0]



    pack $data(w:hlist) -in $data(pw:f1) -expand yes -fill both -padx 0 -pady 0

    set data(w:hsb) \
	[scrollbar $w.hsb -orient horizontal -takefocus 0]
    set data(w:vsb) \
	[scrollbar $w.vsb -orient vertical -takefocus 0]
    
    set data(pw:client) $data(pw:f1)
}

proc tixSHList:SetBindings {w} {
    upvar #0 $w data


    tixChainMethod $w SetBindings

    $data(w:hlist) config \
	-xscrollcommand "$data(w:hsb) set"\
	-yscrollcommand "$data(w:vsb) set"\
	-sizecmd [list tixScrolledWidget:Configure $w]

    $data(w:hsb) config -command "$data(w:hlist) xview"
    $data(w:vsb) config -command "$data(w:hlist) yview"

}
proc tixSHList:GeometryInfo {w mW mH} {
    upvar #0 $w data

    set extra [expr [$w.f1 cget -bd]+[$w.f1 cget -highlightthickness]]

    set mW [expr $mW - $extra*2]
    set mH [expr $mH - $extra*2]

    if {$mW < 1} {
	set mW 1
    }
    if {$mH < 1} {
	set mH 1
    }
    return [$data(w:hlist) geometryinfo $mW $mH]
}
proc tixSHList:RepackHook {w} {
    upvar #0 $w data
    tixChainMethod $w RepackHook
}
#=======================================================
# Filename read/write  functions
#========================================================
# create file "filename" from default values 
proc generate_def {filename} {
    global variable_names
    set fmt [string first ".h" $filename]
    set file [open $::current_project/$filename w]
    foreach varname $variable_names {
        if { $filename == [getfn $varname] } {
            out_line $file $varname $fmt [getdv $varname]
        }
    } 
    close $file
}
proc getline {file line} {
    upvar $line ll
    set ll ""
    while {1} {
       set error [gets $file l]
       if { $error < 0 } break
       if { [string index $l end ] == "\\" } {
           set l [string trim $l "\\ "]
           set ll "$ll $l"
       } else {
           set l [string trim $l]
           set ll "$ll $l"
           break
       }
    }
    set ll [string trim $ll]
    return $error;
}
#read one file set variables 
proc user_setvar1 {filename} {
	if { ![file readable $::current_project/$filename]} then {
            generate_def $filename
            return
        } 
	set file [open $::current_project/$filename r]
    	set fmt [string first ".h" $filename]
	while { [getline $file line] >= 0} {
        set value 0
        if {$fmt > 0} {
           # удалим коментарии 
           if { [regexp {(.*)\/\*(.*)} $line foo varname foo2] } {
             set line $varname
           }
           if { [regexp {^ *\#define +([0-9A-Za-z_]+) +([\.0-9A-Za-z_\+\|\-\*\/\(\)]+).*} $line foo varname value] || \
                [regexp {^ *\#define +([0-9A-Za-z_]+) +\"([^\"]*)\".*} $line foo varname value] || \
                [regexp {^ *\#undef +([0-9A-Za-z_]+).*} $line foo varname] } {
                set ::$varname $value
	       }
        } else {
           if { [regexp {^ *([0-9A-Za-z_]+) *= *(.*)} $line foo varname value] } {
                set ::$varname $value
	       }
        }
	}
	close $file
}
# read files set variables 
proc user_setvar {} {
	foreach i $::filenames {
        user_setvar1 $i
        if ![file writable $i] {
            set ::savestate disabled
        }
    }
    if $::x_enabled {
      .buttons.save configure -state $::savestate
      .buttons.se configure -state $::savestate
    } else {
      save_project $::current_project
      exit 0
    }
}
proc getcomm {file varname str} {
	set l [string length $str]
        set ll [expr [llength [split [getpath $varname] .]]*3]
        set addpad [string repeat " " $ll]
        set comm "/* $addpad[getfuname $varname]"
        set lcomm [string length $comm]
        if { $lcomm < 40 } {
           set footpad [string repeat " " [expr 40-$lcomm]]     
        } else {
           set footpad ""
        }
        set comm "/*! $addpad[getfuname $varname]$footpad */"
        set pad ""
        if { $l < 40 } {
        	set pad [string repeat " " [expr 40 - $l]]
                return "$str$pad$comm"
        } else {
        	set pad [string repeat " " 40]
                puts $file "$pad$comm"
                return $str
        }
}
proc outdef {file varname} {
    if { [set ::$varname] == 1 } { 
		puts $file [getcomm $file $varname "#define $varname 1" ]
    } else {
		puts $file [getcomm $file $varname "#undef $varname" ] 
    }
}

proc out_line { file varname type val } {
    global q
    set space "         "
    set fmt [getfmt $varname]
    if { $type < 0 } {
        if { $fmt != "q" } {
            set fmt m
        }
    } 
    switch $fmt {
       s	{ puts $file [getcomm $file $varname "#define $varname $q$val$q"] }
       d    -
       f    -
       i	{ puts $file [getcomm $file $varname "#define $varname $val"] }
       u	{ outdef $file $varname }
       x	{ puts $file [getcomm $file $varname [format "#define $varname 0x%x" $val]] }
       m    	{ 
                  if { [llength $val] == 0 } {
                     puts $file "$varname = "
                  } elseif { [llength $val] == 1 } {
                     puts $file "$varname = $val"
                  } else {
                     set outline "$varname = "
                     puts -nonewline $file 
                     foreach i $val {
                        if { [string length "$outline $i"] > 80 } {
 		           puts $file "$outline \\"
                           set outline "$space$i"
                        } else {
                           set outline "$outline $i"                        
                        }
                     }
 		     puts $file $outline
                  }
	        }
    }
}
# -----------------------------------------
proc checkline {l} {
	return [string first "/*!" $l]==-1
}
# Записать переменные всех файлов одной цели
proc save_project {project} {
   global variable_names  q
   global filenames
   if [array exists ::pindex] {
    set i  $::pindex($project)      
   set pref p${i}_
   } else {
		set pref ""
   }
   foreach j "$filenames" {
      file rename -force $project/$j $project/$j.old
      set filer [open "$project/$j.old" r]
      set file  [open "$project/$j" w]
      set fmt [string first ".h" $j]
      set out_lined {}
      if {$fmt > 0 } {
          puts $file "/*! Last Saved by OS VERSION $::OS_MAJOR.$::OS_MINOR.$::OS_RELEASE BSP VERSION $::BSP_MAJOR.$::BSP_MINOR.$::BSP_BETA */"
      }
      while { [getline $filer line] >= 0} {
          set value 0
          if {$fmt > 0 } {
              if { [regexp {^ *\#define +([0-9A-Za-z_]+) +([0-9A-Za-z_]*).*} $line foo varname value] || \
                  [regexp {^ *\#define +([0-9A-Za-z_]+) +(\"[^\"]*\").*} $line foo varname value] || \
                  [regexp {^ *\#undef +([0-9A-Za-z_]+).*} $line foo varname]} {
                  if { [lsearch -exact $variable_names $varname ] >= 0 } {
                      out_line $file $varname $fmt [set ::${pref}$varname]
                      lappend out_lined $varname
                  } else {
                          if [checkline $line] {
							puts $file $line
						  }
                  }
              } else {
                      if [checkline $line] {
						puts $file $line
					  }
              }
          } else {
             if { [regexp {^ *([0-9A-Za-z_]+) *= *(.*)} $line foo varname value] } {
                  if { [lsearch -exact $variable_names $varname ] >= 0 } {
                      out_line $file $varname $fmt [set ::${pref}$varname]
                      lappend out_lined $varname
                  } else {
                          puts $file $line
                  }
              } else {
                      puts $file $line
              }
          }
      }
      close $filer
      foreach varname $variable_names {
          if { [getfn $varname]==$j && [lsearch -exact $out_lined $varname ] == -1 } {
                  out_line $file $varname $fmt [set ::${pref}$varname]
          } 
      }
      close $file
   }
}

# Записать переменные всех файлов одной цели
proc save {w} {
   focus $w
   update  
   save_project $::current_project
}
# Записать переменные всех файлов всех целей проекта
proc save_all {w} {
   focus $w
   update  
   foreach project $::projects "save_project $project"
}

proc savevar {} {
	#copy variables to project related variables
        set i $::pindex($::current_project)
        foreach v $::variable_names {
        	set ::p${i}_$v [set ::$v]
        }
}
proc loadvar {} {
        #copy project related variables to variables
        set i $::pindex($::current_project)
	foreach v $::variable_names {
        	set ::$v [set ::p${i}_$v]
        }
}
proc addfile {file} {
    global filenames 
    if { [lsearch $filenames $file] == -1 } {
        lappend filenames $file
    }
}
proc extern_sources {} {
    set es 0
    set retval {}
    foreach file $::argv {
       puts -nonewline "extern sources $file ..."
       if [file readable $file ] {
          namespace eval n$es "source $file"
          puts "OK"
          loadlocale_file  [file rootname $file]
          lappend retval n$es
          incr es
       } else {
          puts "Fail"
       }
    }
    return $retval
}
#=======================================================
# File and other list selection  functions
#========================================================
#
# Вверх 
#
proc dir_up {} {
    global dir_name
    set dir_name [file dirname $dir_name]
    dir_load $::rlist
}
#
# Проверка фильтра
#
proc string_match {dir_filter j} {
	foreach i $dir_filter {
		if [string match $i $j] "return 1"
        }
        return 0
}
#
# Является ли файл каталогом?
#
proc file_isdirectory {fname} {
	return [expr [file isfile $fname]==0]
}
#
# Зарузка списка файлов
#
proc dload {v} {
	if $::dddd return
        set ::dir_name $v
        dir_load $::rlist
}
proc dir_load w {
    global dir_name dir_filter dir_only
    set a {}
    set ::dddd 1
    foreach i [$::tom entries] {
        $::tom delete $i
    }
    set a $dir_name
    foreach i [file split $dir_name] {
        $::tom add command $a
        set a [file dirname $a]
    }
    set ::dname $dir_name
    set ::dddd 0
    $w delete 0 end
    set a  [lsort [exec ls $dir_name]]
	#
	# Сачала директории
	#
    foreach i  $a {
		if { [file_isdirectory [file join $dir_name $i]] } {
			$w insert end $i/
		}
    }
	#
	# Потом файлы 
	#
    if { $dir_only == "0" } {
            foreach i  $a {
                    set j [string trim $i "\@"]
                    if { ![file_isdirectory [file join $dir_name $j]] && [string_match $dir_filter $j] } {
                            $w insert end $j
                    }
            }
    }
}
proc dir_remove {w} {
  set a [$::llist  curselection]
  set a [lsort -decreasing $a]
  foreach i $a {
        $::llist delete $i
  }
}
proc dir_add {w} {
    global dir_name one_file dir_only
   set a [$::rlist curselection]
   foreach i $a {
     set b [file join $dir_name [$::rlist get $i]]
	 if { $dir_only == "0" } {
		if { [file_isdirectory $b] } {
		} else {
			$::llist insert end $b   
		}
	 } else {
		if { [file_isdirectory $b] } {
			$::llist insert end $b   
		} else {
		}
	 }
   }
   set a [$::llist get 0 end]
   $::llist delete 0 end
   if { $one_file == "1" } {
	   $::llist delete 0 end
	   $::llist insert end [lindex $a 0]
   } else {
	foreach i $a {
   		$::llist insert end $i
	}
   }
}
proc dir_dblclk {} {
    global dir_name one_file dir_only
    set f [file join $dir_name [$::rlist get active]]
	if { [file_isdirectory $f] } {
		set dir_name $f
		dir_load $::rlist
	} else {
		if {  $one_file == "1"  } {
			$::llist delete 0 end
		}
   		$::llist insert end $f
	}
}
proc dir_ok {w entry} {
    $entry delete 0 end
    $entry insert 0 [$::llist get 0 end]
    $entry validate
    set ::fdresult 1
    destroy $w
}
proc dir_up {w} {
	set sel [$::llist curselection]
        if [llength $sel] {
        	set sel [lindex $sel 0]
        	if { $sel > 0 } {
			set val [$::llist get $sel]
                        $::llist delete $sel $sel
                	set sel [expr $sel-1]
                        $::llist insert $sel $val
                	$::llist selection set $sel
                }
        }
}
proc dir_down {w} {
	set sel [$::llist curselection]
        if [llength $sel] {
        	set sel [lindex $sel 0]
                set val [$::llist get $sel]
                $::llist delete $sel $sel
                set sel [expr $sel+1]
                $::llist insert $sel $val
                $::llist selection set $sel
        }
}
proc dir_set_filter {filter} {
	set ::dir_filter $filter
	dir_load $::rlist
}
proc file_dialog {entry filemask one_file dir_only} {

  set ::one_file $one_file
  set ::dir_only $dir_only 
  global rlist llist
  set ::fdresult 0
  set w .a
  catch {destroy $w}
  toplevel $w
  wm title $w ""
  # [lcget "Select files from listbox"]

#============= Bottom frame ===========================

frame $w.buttons
pack $w.buttons -side bottom -fill x -pady 2m


#============= Top frame ===========================
frame $w.top
pack $w.top -side top -fill x

set ::tom [tixOptionMenu $w.top.dir_name -width 30 -variable dname -command "dload"]
pack $w.top.dir_name -side left -expand yes -fill both
button $w.top.up -text [lcget Up] -command {dload [file dirname $::dir_name]}
pack $w.top.up -side right
#============ Middle frames ====================== 

tixPanedWindow $w.f -orientation horizontal -panebd 3 -panerelief sunken
pack $w.f -side top -expand yes -fill both

$w.f add l 
$w.f add r

set llist [[tixScrolledListBox  $w.f.l.list  ] subwidget listbox]
$llist  configure -selectmode extended -width 40 -height 20 
pack $w.f.l.list -side left -expand 1 -fill both

set rlist [[tixScrolledListBox $w.f.r.list  ] subwidget listbox]
$rlist configure  -selectmode extended  -width 20 -height 20
pack $w.f.r.list -side left -expand 1 -fill both

#============= Bottom frame ===========================

button $w.buttons.ok -text [lcget Ok] -command "dir_ok $w $entry"
button $w.buttons.remove -text [lcget Remove] -command "dir_remove $w"
button $w.buttons.add -text [lcget Add] -command "dir_add $w"
button $w.buttons.up -text [lcget Up] -command "dir_up $w"
button $w.buttons.down -text [lcget Down] -command "dir_down $w"

button $w.buttons.cancel -text [lcget "Cancel"] -command "destroy $w"


set m [tixOptionMenu $w.buttons.filter  -command "dir_set_filter"]
# -label [lcget "Filter"] 
$m add command {*.c *.s *.S} -label "[lcget {Source files}] .c .s .S" 
$m add command {*.o} -label "[lcget {Object files}] .o" 
$m add command {*.a} -label "[lcget {Archive files}] .a" 
$m add command {*} -label "[lcget {All files}]" 
catch {
$m configure -value $filemask
}
set ::dir_filter $filemask

pack $w.buttons.ok $w.buttons.remove $w.buttons.add $w.buttons.up $w.buttons.down $w.buttons.cancel $w.buttons.filter -side left -expand 1


$llist delete 0 end
foreach i [$entry get] {
        $llist insert end  $i
}
dir_load $rlist

bind $rlist <Double-Button-1> "dir_dblclk"
bind $w.top.dir_name <Return> "dir_load $rlist"
return $w
}


proc list_add { w } {
   global list_a
   if { $list_a != "" } {
	    $::llist insert end $list_a
   } else {
        set a [$::rlist curselection]
        if { $a != "" } {
            foreach i $a { $::llist insert end [$::rlist get $i] }
        }
   }
   set a [$::llist get 0 end]
   $::llist delete 0 end
   foreach i $a { $::llist insert end $i }
}


proc list_dialog {entry list} {

  set w .a
  global list_a  llist rlist
  set list_a ""
  catch {destroy $w}
  toplevel $w
  wm title $w ""
  # [lcget "Select from listbox"]

#============= Bottom frame ===========================

frame $w.buttons
pack $w.buttons -side bottom -fill x -pady 2m

button $w.buttons.ok -text [lcget Ok] -command "dir_ok $w $entry"
button $w.buttons.remove -text [lcget Remove] -command "dir_remove $w"
button $w.buttons.add -text [lcget Add] -command "list_add $w"

button $w.buttons.cancel -text [lcget "Cancel"] -command "destroy $w"

pack $w.buttons.ok $w.buttons.remove $w.buttons.add $w.buttons.cancel -side left -expand 1

#============ Middle frames ====================== 

tixPanedWindow $w.f -orientation horizontal -panebd 3 -panerelief sunken
pack $w.f -side top -expand yes -fill both

$w.f add l 
$w.f add r

set llist [[tixScrolledListBox  $w.f.l.list  ] subwidget listbox]
$llist  configure -selectmode extended -width 40 -height 20 
pack $w.f.l.list -side left -expand 1 -fill both

set rlist [[tixScrolledListBox $w.f.r.list  ] subwidget listbox]
$rlist configure  -selectmode extended  -width 20 -height 20
pack $w.f.r.list -side left -expand 1 -fill both

$::llist delete 0 end
foreach i [$entry get] {  
        $::llist insert end  $i 
}
foreach i  $list { $::rlist insert end $i }
			
bind $::rlist <Double-Button-1> "list_add $w"
return $w
}
#=======================================================
# Properties functions
#========================================================
proc getprop {varname prop} {
    global variable_props 
    if {$varname == ""} return ""
    if {[catch {set variable_props($varname)} a]} {
    	set a  [set variable_props(${varname}0)]
    }
    return [lindex $a $prop]
}
proc addvar { varname i fn fmt dv pn cl rl vrexp vcmd msg url urlbase group name path optlist kbmb et} {
    global variable_names variable_props
    global current_val x_enabled
    if {[catch {set variable_props($varname)}]==0} {
    	set msg "Fatal: duplicate variable $varname"
    	if $x_enabled {
		tk_messageBox  -type ok -message $msg
        } else {
		puts "$msg"
        }
        exit 1
  	
    }
    if { $group != {} } {
        lappend ::groups_$group $varname
    } 
    lappend variable_names $varname
    if { $current_val != {} && $pn >= 0 && [lindex [lindex $current_val $i] $pn] != {} } {
        set dv [lindex [lindex $current_val $i] $pn]
    }
    set ::$varname $dv
    lappend a $fn
    lappend a $fmt
    lappend a $cl
    lappend a $rl
    lappend a $dv
    lappend a $vrexp
    lappend a $vcmd
    lappend a $msg
    lappend a $url
    lappend a $urlbase
    lappend a $name
    lappend a $path
    lappend a $optlist
    lappend a $kbmb
    lappend a $i
    lappend a $et
    lappend a $group
    set variable_props($varname) $a
}

proc getfn {varname} {
    return [getprop $varname 0]
}
proc getfmt {varname} {
    return [getprop $varname 1]
}
proc getcond {varname} {
    return [getprop $varname 2]
}
proc getref {varname} {
    return [getprop $varname 3]
}
proc getdv {varname} {
    return [getprop $varname 4]
}
proc getvrexp {varname} {
    return [getprop $varname 5]
}
proc getvcmd {varname} {
    return [getprop $varname 6]
}
proc getmsg {varname} {
    return [lcget [getprop $varname 7]]
}
proc geturl {varname} {
    return [getprop $varname 8]
}
proc geturlbase {varname} {
    return [getprop $varname 9]
}
proc getname {varname} {
    set n  [getprop $varname 10]
    return [lcget $n]
}
proc getfuname {varname} {
    set n  [getprop $varname 10]
    if { $n == "" } "set n $varname"
    set n [lcget $n]
    set i [getprop $varname 14]
    if { $i != "" } {
    	set n "${n}(${i})"
    }
    return $n
}
proc getpath {varname} {
    return [getprop $varname 11]
}
proc getopt {varname} {
    return [getprop $varname 12]
}
proc getkbmb {varname} {
    return [getprop $varname 13]
}
proc getet {varname} {
    return [getprop $varname 15]
}

proc getdva {varname i} {
	set dv [getopt $varname]
	if { $dv == {} } {
		return "[expr $i == 0 ]"
	} 
    incr i
    set r "[expr ![expr [lsearch $dv $i] == -1 ]]"
	return $r
}
proc getgroup {varname} {
    return [getprop $varname 16]
}
#=======================================================
# Trace and validation functions
#========================================================
proc oneof {v p } {
   if { $v == 1 } {
      foreach i $p  {
        set ::$i 0
      }
    }
    return 1
}
proc uuu {c} {
   if [regexp {([^\$]*)\$([_a-zA-z][0-9_a-zA-Z]*)(.*)} $c foo pref v suff] {
           foreach i [split [getpath $v] .] {
                   set n [getname $i]
                        if [string length $n] {
                           lappend names $n
                        }
                }
#           return "$pref[join $names .].[getname $v]$suff"
           return "$pref[join $names .]$suff"
        }
        return $c
}
proc check_cond {varname} {
    set cond [getcond $varname ]
    if {$cond != {}} {
   foreach c [split $cond &] {
                if { $c!={} && [uplevel #0 expr $c] == 0 } {
         set ::var_emsg($varname) "[lcget {false condition:}] [uuu $c]"
                        return 0
                }
        }
    }
    return 1
}

proc warn_dlg msg {
   tk_messageBox  -type ok -message "False condition $msg"
}
proc bool_validate {varname sc w} {
    set ::curvar $varname
    focus $w
    global $varname 
    set v [set $varname]

    set vcmd [getvcmd $varname]
    if { $vcmd != {} } {
        set ::P $v   
        if { [uplevel #0 expr $vcmd] == 0 } {
          # bad
          fobell $varname $w $vcmd
          return 1
        }
        screen_setstate
    }

    set group [getgroup $varname]
    if { $group != {} } {
        one_of $varname [set ::groups_$group]
    }
    if { $sc != -1 } {
       global thl
        set ::$varname$::current_line $v
        $thl item configure $::current_line [expr $sc+1] -image [lindex  $::ck_list $v]
    }
    set cond [getcond $varname]
    # we try to turn on variable 
    # check for condition whether we can do it
    if ![ check_cond $varname ] {
        set $varname 0
        warn_dlg $cond
        return
    }
    set ref [getref $varname ]
    set vv_list {}
    if {$ref != {}} {
       foreach i $ref {
         global $i
         if ![check_cond $i ] {
          if { [set $i]==1 } { lappend vv_list $i }
         } 
       }
    }
    # we change variable value 
    # check for reference condition 
    # whether we can turn off other variables 
    if { $vv_list != {} } {
       set answer [tk_messageBox  -message "This will turn off variables: $vv_list" -type yesno -icon question]
       if { $answer == "yes" } {
            foreach i $vv_list "set ::$i 0"
       } else {
          set v  [expr $v?0:1 ]
            set $varname $v
       }
    }
    after idle screen_setstate
}
#=======================================================
# Condition reference   functions
#========================================================

proc global_validation { P V varname sc w} {
   # check for state of window
        #
   if { [all_enabled $varname]==0 || [check_cond $varname]==0 } {
           return 1
        }
        set rtn 1
        switch $V {
           key "set rtn [key_validation $P $varname $sc $w]"
               forced -
           focusout "set rtn [fo_validation $P $varname $sc $w]"
                focusin "set rtn [fi_validation $P $varname $sc $w]"
        }
        # puts "global valid $P $V $varname $sc $w ret $rtn"
        return $rtn
}
proc key_validation_fo { P varname sc w } {
        if {[getopt $varname] != {} } {return 1}
        # check for user defined regular expression 
        set vrexp [getvrexp $varname]
        if { $vrexp == {} } {
            # check for variable format 
            set fmt [getfmt $varname]
         # puts "KV0 fmt  $P $varname $sc $fmt"
         switch $fmt {
               i -
               x
               { 
                   return [regexp {^[\-]?(0x)?[0-9a-fA-F]*[kKmM]?$} $P] 
               }
               f
               { 
                   return [regexp {^[0-9]*\.?[0-9]*$} $P] 
               }
         }
        } else {
         # puts "KV0 re  $P $varname $sc $vrexp "
           return [regexp $vrexp $P]
        }
   # puts "KV0 OK!!!"
        return 1
}
proc key_validation { P varname sc w} {
      return 1
      # allow temporary empty values
      if { $P=="" } {return 1}
      return [key_validation_fo $P $varname $sc $w]
}
proc getm {m} {
   switch $m {
           k -
                K "return 1024"
                m -
                M "return 1048576"
                default "return 1"
        }
}
proc setval {w P} {
    
    if { $w != "" } {
        if { [winfo class $w] == "TixComboBox" } {
           set w [$w subwidget entry]
        } 
        set v [$w cget -validate]
        $w configure -validate none
        $w delete 0 end
        $w insert 0 $P
        # after idle "$w configure -validate $v"
        $w configure -validate $v
    }
}
proc fobell {varname w fmt} {
   if {[catch {set ::$varname} v]} {
           set v [set ::${varname}${::current_line}]
        }
        setval $w $v
        puts "$varname $v $fmt --- bell"
        bell
}
#
# Focus out validation
#
proc fo_validation { P varname sc w} {
        # check variable value presence 
        # in option list
        # 
        if {[getopt $varname] != {} } {
            # OK set varname as entry value
            if { $sc == -1 } {
                    set ::$varname $P
            } else {
                    global thl
                    set ::$varname$::current_line $P
                    $thl item configure $::current_line [expr $sc+1] -text $P
            }
            return 1
        }
        # puts "FO $P $varname $sc [getfmt $varname]"
        if ![key_validation_fo $P $varname $sc $w] {
           # bad
           fobell $varname $w [getfmt $varname]
           return 1
        }
        # Convert k or m for int types
        switch [getfmt $varname]  {
           i -
                x { 
                    if [regexp {([\-]?[0-9xa-fA-F]+)([kKmM])} $P foo v m ] {
                       set P [expr $v*[getm $m]]
                    }
                }
        }
        switch [getfmt $varname]  {
           i { 
                       set P [format %d $P]
                       setval $w $P
                }
           x { 
                       set P [format 0x%x $P]
                       setval $w $P
                }
        }
        # check for user validation function
        # 
        set vcmd [getvcmd $varname]
        if { $vcmd != {} } {
            set ::P $P   
            if { [uplevel #0 expr $vcmd] == 0 } {
              # bad
              fobell $varname $w $vcmd
              return 1
            }
            screen_setstate
        }
        # OK set varname as entry value
        if { $sc == -1 } {
           set ::$varname $P
        } else {
                global thl
                set ::$varname$::current_line $P
                $thl item configure $::current_line [expr $sc+1] -text $P
        }
        # we change value so ...
        screen_setstate
        return 1
}
#
# Focus in validation
#
proc fi_validation { P varname sc w} {
   set ::curvar $varname
        if { ![string is integer $P ]|| $P < 1024 } {
            return 1    
        }
        # Convert int types to k or m 
        set m {}
        if [getkbmb $varname] {
           if { [expr $P%1048576]==0} {
                   set P [expr $P/1048576]
                        set m M
                }
           if { [expr $P%1024]==0} {
                   set P [expr $P/1024]
                        set m K
                }
        }
        if { $m != {} } {
            set P [format %i%s $P $m]
            setval $w $P
        }
        return 1
}
proc ref_validate {w varname sc v } {
   # check for state of window
   #
   if { [all_enabled $varname]==0 || [check_cond $varname]==0 } {
      return 1
   }
   if { [fo_validation $v $varname $sc $w]!=1 } {
           return 1
   }
   if { [getref $varname]!= {} } screen_setstate
   return 1
}
proc range {v min max} {
   return [expr $v >= $min && $v <= $max ]
}
proc ipvalidate {P} {
    if [regexp {^([0-9]{1,3})\.([0-9]{1,3})\.([0-9]{1,3})\.([0-9]{1,3})$} $P foo v1 v2 v3 v4 ] {
       if { [range $v1 0 255] && [range $v2 0 255] && [range $v3 0 255] && [range $v4 0 255] } {
           return 1
        }
    }
    return 0
}
proc all_enabled {varname} {
   foreach v [split [getpath $varname] .] {
           if {$v==$varname} {
#   puts "all_enabled EOL ret 1"
              return 1
           }
           set vv 1
           catch {
              set vv [set ::var_states($v)]
           }
           if {[set ::$v]==0 || $vv==0} {
#   puts "all_enabled $v==0 ret 0"
      set ::var_emsg($varname) "[lcget {false uplevel condition:}] [getname $v]"
              return 0
           }
   }
}
proc screen_setstate {} {
   global varnames winnames 
   foreach i $varnames {
      set idx [lsearch $varnames $i]
      if { $idx != -1 } {
          set w [lindex $winnames $idx]
# puts "screen_setstate $i $w"          
          if [all_enabled $i] {
             if [check_cond $i] {
                 catch "$w configure -state normal"
                 set ::var_emsg($i) [getmsg $i]
                 set ::var_states($i) 1
             } else {
                 catch "$w configure -state disabled"
#                set ::var_emsg($i) "false condition: [getcond $i]"
                 set ::var_states($i) 0
             }
          } else {
             catch "$w configure -state disabled"
            set ::var_states($i) 0
          }
      } 
   }
}
proc setemsg {varname type} {
   switch $type {
      Leave "$::status configure -text {}"
        Enter {
           set emsg [getmsg $varname]
                catch "[set emsg $::var_emsg($varname)]"
                $::status configure -text $emsg
        }
   }
#   return $::var_emsg($varname)
}
proc setvar {varname val} {
   set ::$varname $val
   set w [getwin $varname]
    if { $w != "" } {
        if { [winfo class $w] == "TixLabelEntry" } {
            set e [$w subwidget entry]
            $e delete 0 end
            $e insert 0 $val
        }
    }
    return 1
}
proc one_of {varname varlist} {
   if { [set ::$varname] } {
      foreach a $varlist {
         if { $a != $varname } {
            set ::$a 0
         }
      }
    }
    return 1
}
proc addline {thl varname} {
        $thl add $varname -data $varname
        $thl item create $varname 0 -text [getfuname $varname]
        $thl item create $varname 1 -text [getdv $varname]
        set i 0
        foreach j $::projects {
                $thl item create $varname [expr $i+2] -text [set ::p${i}_$varname]
                incr i
        }
}

proc show_all {w} {
 	
        focus $w
	update  
    # idletasks
        savevar
        toplevel .a
        wm geometry .a =800x600
        wm title .a "Projects variables"
        set w .a
#============= Top frame ===========================
   	frame $w.top  -relief raised -borderwidth 2
   	pack $w.top -side top -fill both -expand yes
        set list [tixScrolledHList $w.top.l -columns [expr $::nprojects+2]]
        set thl [ $list subwidget hlist]
        $thl configure -header 1 -selectmode multiple -command "showline $thl"
        $thl header create 0  -text [lcget "Variable name"] 
	# $thl column width 0  -char 40
        $thl header create 1  -text [lcget "Default value"]
        set i 2
        foreach j $::projects {
        	$thl header create $i  -text $j
                incr i
        }
        show_full $thl
        pack $list -fill both -expand yes
#============= Bottom frame ===========================
   	frame $w.buttons -relief raised -borderwidth 2
   	pack $w.buttons -side bottom -fill x 
   	button $w.buttons.b1 -text [lcget "Diffrerence"] -command "show_diff $thl"
   	button $w.buttons.b2 -text [lcget "Full list"] -command "show_full $thl"
   	button $w.buttons.b3 -text [lcget "Set default"] -command "set_default $thl"
	menubutton $w.buttons.b4 -text [lcget "Set like"] -underline 0 -direction below -menu $w.buttons.b4.m -relief raised
	menu $w.buttons.b4.m -tearoff 0 
	
        foreach j $::projects {
        	$w.buttons.b4.m add command -label $j -command "set_like $thl $j"
	}   	
        button $w.buttons.cancel -text [lcget "Close"] -command "destroy $w"
   	pack $w.buttons.b1 $w.buttons.b2 $w.buttons.b3 $w.buttons.b4 $w.buttons.cancel -side left -expand 1
}
proc hasdiffdv {varname} {
	set dv [getdv $varname]
      	for {set i 0} {$i<$::nprojects} {incr i} {
        	if { $dv != [set ::p${i}_$varname]} "return 1"
        }
        return 0
}
proc hasdiff {varname} {
	set dv [set ::p0_$varname]
      	for {set i 1} {$i<$::nprojects} {incr i} {
        	if { $dv != [set ::p${i}_$varname]} "return 1"
        }
        return 0
}
proc show_diff {thl} {
      $thl delete all
    	global variable_names
    	foreach varname $variable_names {
                if [hasdiff $varname] {
			addline $thl $varname
                }
        }
      
}
proc show_full {thl} {
      $thl delete all
    	global variable_names
    	foreach varname $variable_names {
#        	if [string equal -length [string length $::oldpath] $::oldpath [getpath $varname] ] 
        	if [string match ${::oldpath}* [getpath $varname] ] {
		addline $thl $varname
                }
        }
      
}
proc set_default {thl} {
	set sel [$thl info selection ]
        foreach varname $sel {
        	set dv [getdv $varname]
      		for {set i 0} {$i<$::nprojects} {incr i} {
        		set ::p${i}_$varname $dv
                	$thl item configure $varname [expr $i+2] -text [set ::p${i}_$varname]
        	}
                
        }
        loadvar
}
proc set_like {thl proj} {
	set sel [$thl info selection ]
        set i $::pindex($proj)
        foreach varname $sel {
        	set dv [set ::p${i}_$varname]
      		for {set i 0} {$i<$::nprojects} {incr i} {
        		set ::p${i}_$varname $dv
                	$thl item configure $varname [expr $i+2] -text [set ::p${i}_$varname]
        	}
                
        }
        loadvar
}
proc showline0 {varname} {
        toplevel .b
#        wm geometry .b =300x200
        set w .b
#        set list [tixScrolledHList $w.l -columns 2]
#        set thl [ $list subwidget hlist]
        set thl [tixHList $w.l -columns 2 -height 5 -width 80]
        $thl configure -header 1 
        $thl header create 0  -text [lcget "Project"]
        $thl header create 1  -text [getname $varname]
        set i 0
        foreach j $::projects {
        	$thl add $i
                $thl item create $i 0 -text $j
                $thl item create $i 1 -text [set ::p${i}_$varname]
                incr i
        }
        pack $w.l 
        # -expand yes 
        # -fill both
}
proc showline {thl varname } {
	if [winfo exists .b ] {
        	 wm deiconify .b
                 return
        }
        toplevel .b
        frame .b.top
        pack  .b.top
        set w .b.top
	label  $w.l  -text [lcget "Project"]
	label  $w.l1  -text [getfuname $varname]
        grid  $w.l $w.l1
        set i 0
        foreach j $::projects {
                label $w.z$i -text $j
                set e [et $w $i $varname]
        	grid  $w.z$i $e
                incr i
        }
        frame .b.bottom
        pack  .b.bottom
        set w .b.bottom
        button  $w.ok  -text [lcget "Ok"] -command "DiffOk $varname $thl"
        button  $w.cancel -text [lcget "Cancel"] -command "destroy .b"
        pack 	$w.ok  $w.cancel -side left -expand 1
        
        # -expand yes 
        # -fill both
}                                                                                                           
proc et { w i varname } {
	set ::cbv$i [set ::p${i}_$varname]
	switch [getet $varname] {
        	bool {
                	set e [checkbutton $w.e$i -variable cbv$i]
                }
        	combo  {
                	set e [tixComboBox $w.e$i -variable cbv$i]
                        $e subwidget entry configure -width 40 
                        foreach v [getopt $varname] {
                           $e appendhistory $v
                        }
                }
                int {
			set e [entry $w.e$i -width 20 -textvariable cbv$i -validate key -invcmd "mybell $varname %W" -vcmd "key_validation %P $varname -1 %W"]
                }
                text -
                default {
			set e [entry $w.e$i -width 40 -textvariable cbv$i]
                }
        }
        return $e
}
proc DiffOk {varname thl} {
        set i 0
        foreach j $::projects {
                set ::p${i}_$varname [set ::cbv$i]
                if { $j == $::current_project } {
                   set ::$varname [set ::cbv$i]     
                }
		$thl item configure $varname [expr $i+2] -text [set ::cbv$i]
                incr i
        }
        destroy .b
        browse0 $::oldpath
}
proc loadlocale_file {basename} {
    set enc [encoding system]
    set filename "$basename.lc.$enc"
    puts -nonewline "locale $filename ..."
    if [file exists $filename] {
       set file  [open "$filename" r]
       while { [gets $file line] >= 0} {
           lappend ::enames [lindex $line 0]
           lappend ::lcnames [lindex $line 1]
       }
       puts "OK"
    } else {
       puts "Fail"
    }
    
}
proc loadlocale {  } {
    set ::enames {}
    set startdir [file dirname $::argv0]
    lappend filenames $startdir/internal
    lappend filenames "[file rootname $::argv0]"
    
    foreach filename $filenames {
       loadlocale_file $filename
    }
}
proc lcget  { ename } {
    set idx [lsearch $::enames $ename]
    if { $idx==-1} {
    	return $ename
    }
    set name [lindex $::lcnames $idx]
    if { $name == "" } {
    	return $ename
    }
    return $name
}
set nprojects 0
proc loadproject_fromvar {} {
	set ::nprojects 0
        set i 0
        set w .buttons.lb
        set lb [$w subwidget listbox ]
        $lb delete 0 end
        catch "unset ::pindex"
    	foreach line $::projects {
        	.buttons.lb appendhistory $line
                set ::current_project $line
                # set variables to default values
		foreach v $::variable_names {
                        set ::$v [getdv $v]
                }
                #load variables from files
                user_setvar
                #copy variables to project related variables
		foreach v $::variable_names {
                        set ::p${i}_$v [set ::$v]
                }
                set ::pindex($line) $i 
                incr i
                incr ::nprojects 
        }
        after idle {}
        if [expr $::nprojects > 0 ] {
		.buttons.lb configure -state normal
                .buttons.saveall configure -state normal
                .buttons.show  configure -state normal
                .buttons.proj.m entryconfigure 1 -state normal
		.buttons.proj.m entryconfigure 2 -state normal                
                .buttons.lb pick 0
        }
}
proc loadproject {filename} {
     if { $::x_enabled == 0 } return
     set ::projects {}
     if [file readable $filename] {
        set ::projectfilename $filename
        set file  [open $filename r]
        while { [gets $file line] >= 0} {
                if [string match #* $line] continue
                lappend ::projects $line
        }
        close $file
        loadproject_fromvar
     }
}
set current_project "."
set projectfilename ""
proc project_button_load {w} {
        set e [entry .entry12345]
        set w [file_dialog  $e *.prj 1 0]
        tkwait window $w
        if { [file readable [$e get]] && $::fdresult} {
                loadproject [$e get]
        }
        destroy $e
}
proc project_button_change {w} {
	if ![file writable $::projectfilename] return
        entry .entry12345 -textvariable projects
        set w [file_dialog  .entry12345 a 0 1]
        tkwait window $w
        if $::fdresult {
        	loadproject_fromvar
        }
        destroy .entry12345
}
proc project_button_save {w} {
	if ![file writable $::projectfilename] return
        set file  [open $::projectfilename w]
        foreach i $::projects {
        	puts $file $i
        }
        close $file
}
proc setproject {value} {
#	puts "set project to $value"
        if { $::current_project != "." } savevar
        #copy project related variables to variables
	set ::current_project $value
        loadvar
	#redisplay screen
	set path $::oldpath
        set ::oldpath 0
	browse $path
        wm title . $::current_project
}
#
# ip widget
#
proc common_case {} {
   uplevel  {
      switch $nd {
           valid {set r1 1; set r2 223}
           default {set r1 0; set r2 255}
      }
      if ![range $rdigi $r1 $r2] {
          tk_messageBox -parent $w -type ok -icon warning -message "$rdigi is not a valid entry. Please specify a value between $r1 and $r2"
          ip_setval $w $digits fi $pos
          return 0
      }
      set digits [lreplace $digits $nd $nd $rdigi]
      ip_setval $w $digits fi $pos
   }
}
proc nextchunk {} {
    uplevel {
        # go to next chunk 
        # select it
        if [regexp {[0-2]} $nd] {
            set start  [expr ($nd+1)*4]
            $w selection range $start [expr $start+3]
            $w icursor [expr $start+3]
         }
    }
}
proc ip_setval {w digits f pos} {
	foreach d $digits {
	  set d1  [string trimleft $d " 0"]
          switch $f {
             fi {
          	if { $d1 == "" } {set d1 "0"}
          	lappend ndigits [string range "   $d1" end-2 end]
             }
             fo {
          	if { $d1 == "" } {set d1 "0"}
                lappend ndigits $d1
             }
             work {
          	lappend ndigits [string range "   $d1" end-2 end]
             }
          }
        }
	$w delete 0 end
        $w insert 0 [join $ndigits .]
   	$w icursor $pos
}
proc {ip_spec_binding} {w keysym sym} {
set pos [$w index insert]
set val [$w get]
# process selection
# replace selected syms with spaces
#
if [$w selection present] {
   set sel [string range $val [$w index sel.first] [expr [$w index sel.last]-1]]
   if { [string first . $sel]==-1 } {
   $w delete sel.first sel.last
   ip_setval $w [split [$w get] .] fi $pos
   }
   $w selection clear
}
set val [$w get]
# chunk number
set nd [expr $pos/4]   		
# position inside chunk
set dpos [expr $pos%4] 		
# list of chunks 
set digits [split $val .]       
# value of current chunk 
set digi [string trimleft [lindex $digits $nd] " 0"] 
switch -regexp -- $keysym {
    [0-9] {
        switch $dpos {
               3 { # insertion point
                   switch [string length $digi] {
                        0 -
                        2 -
                   	1 { # insert char before
                            set rdigi "$digi$keysym"
                            common_case
                        }
                        3 nextchunk
                   }
               }
               2 { # before insertion point
                   switch [string length $digi] {
                   	1 {
                          set rdigi "$keysym$digi"
                          common_case
                        }
                        2 {
                          set rdigi "[string index $digi 0]$keysym[string index $digi 1]"
                          common_case
                        }
                        3 nextchunk
                   }
               }
               1 { # before before insertion point
                   switch [string length $digi] {
                        2 {
                          set rdigi "$keysym$digi"
                          common_case
                        }
                        3 nextchunk
                   }
               }
        }
        return 0
    }
    BackSpace {
        switch $dpos {
               3 { # insertion point
                   switch [string length $digi] {
                        3 -
                        2 -
                   	1 { # delete char from end
                            set rdigi " [string range $digi 0 end-1]"
          		    set digits [lreplace $digits $nd $nd $rdigi]
	  		    ip_setval $w $digits fi $pos
                        }
                   }
               }
               2 { # before insertion point
                   switch [string length $digi] {
                        3 { # delete char from end-1
			    set rdigi " [string index $digi 0][string index $digi 2]"                      
          		    set digits [lreplace $digits $nd $nd $rdigi]
	  		    ip_setval $w $digits fi $pos
                        }
                        2 { # delete char from end-1
			    set rdigi "  [string index $digi 1]"                      
          		    set digits [lreplace $digits $nd $nd $rdigi]
	  		    ip_setval $w $digits fi $pos
                        }
                   }
               }
               1 { # after first digit
                   switch [string length $digi] {
                        3 { # delete first char 
			    set rdigi " [string range $digi 1 end]"                      
          		    set digits [lreplace $digits $nd $nd $rdigi]
	  		    ip_setval $w $digits fi $pos
                        }
                   }
               }
        }
    	return 0
    }
    Delete {
    	if [regexp {[0-9]} [string index $val $pos]] {
        $w icursor [expr $pos+1]
        ip_spec_binding $w BackSpace 0
        }
    }
    	
    FocusIn {
    	ip_setval $w [split $val .] fi 3
        return 0
    }
    FocusOut {
    	ip_setval $w [split $val .] fo 0
        $w validate
        return 0
    }
    period { # go to next chunk 
             # select it
             if [regexp {[0-2]} $nd] {
                set start  [expr ($nd+1)*4]
                $w selection range $start [expr $start+3]
                $w icursor [expr $start+3]
             }
       return 0;
    }
    Left {
    	if { $pos==0 } "return 0"
	set i [expr $pos-1]        
        for {} {$i>0} {} {
            if [string match {[0-9.]} [string index $val $i]] {
                $w icursor $i
                return 0
            }
            set i [expr $i-1]
        }
        return 0
    }
    Right {
    	if { $pos==15 } "return 0"
        set i [expr $pos+1]
        for {} {$i<=15} {incr i} {
            if { $i==15 || [string match {[0-9.]} [string index $val $i]] } {
                $w icursor $i
                return 0
            }
        }
        return 0
    }
    Home -
    End -
    Right -
    Up -
    Tab -
    Down  {return 1}
}
return 0
}
proc create_ip { w source_line cn varname sc } {
	global $varname
	set f $w.f$source_line
    frame $f 
    set text [getname $varname]
	set le [tixLabelEntry $f.le -label $text]
	$le subwidget label configure -width $::labelwidth -anchor w -wraplength $::plabelwidth -justify left
        set e  [$le subwidget entry]
    	$e configure -width 15
    	$e configure -validate none -invcmd "mybell $varname %W" -vcmd "global_validation %P %V $varname $sc"
	catch "$e insert 0 [set ::$varname]"
	bind $e <Key> {
		if {[ip_spec_binding %W %K %A]} {continue} else {break}
	}
	bind $e <FocusIn> "ip_spec_binding %W FocusIn 0"
	bind $e <FocusOut> "ip_spec_binding %W FocusOut 0"
	pack $le -anchor w -side left
        pack $f -pady 1 -fill x -anchor w
    	add_column $cn $text
        addmsg $varname $f $le
        addhlp $varname $f
}

# States: normal numbers: for dropdown style
#         d+digit(s)    : for non-dropdown style
#
proc tixComboBoxA:InitWidgetRec {w} {
    upvar #0 $w data

    tixChainMethod $w InitWidgetRec

    set data(curIndex)    ""
    set data(varInited)	  0
    set data(state)       none
    set data(ignore)      0

    if {![string compare $data(-arrowbitmap) ""]} {
	set data(-arrowbitmap) [tix getbitmap cbxarrow]
    }
    if {![string compare $data(-crossbitmap) ""]} {
	set data(-crossbitmap) [tix getbitmap cross]
    }
    if {![string compare $data(-tickbitmap) ""]} {
	set data(-tickbitmap) [tix getbitmap tick]
    }
}

proc tixComboBoxA:ConstructFramedWidget {w frame} {
    upvar #0 $w data

    tixChainMethod $w ConstructFramedWidget $frame

    tixComboBoxA:ConstructEntryFrame $w $frame
    tixComboBoxA:ConstructListShell $w
}


proc tixComboBoxA:ConstructEntryFrame {w frame} {
    upvar #0 $w data

    # (1) The entry
    #
    set data(w:entry) [entry $frame.entry]

    if {!$data(-editable)} {
	set bg [$w cget -bg]
	$data(w:entry) config -bg $bg -state disabled -takefocus 1
    }

    # This is used during "config-state"
    #
    set data(entryfg) [$data(w:entry) cget -fg]

    # (2) The dropdown button, not necessary when not in dropdown mode
    #
    set data(w:arrow) [button $frame.arrow -bitmap $data(-arrowbitmap)]
    if {!$data(-dropdown)} {
	set xframe [frame $frame.xframe -width 19]
    }

    if {$data(-dropdown)} {
	pack $data(w:arrow) -side right -padx 1
	foreach wid "$data(w:frame) $data(w:label)" {
	    tixAddBindTag $wid TixComboWidA
	    tixSetMegaWidget $wid $w TixComboBoxA
	}
    } else {
	pack $xframe -side right -padx 1
    }
    pack $frame.entry -side right -fill x -expand yes -padx 1
}

proc tixComboBoxA:ConstructListShell {w} {
    upvar #0 $w data

    # Create the shell and the list
    #------------------------------
    set data(w:shell) [menu $w.shell -bd 2 -relief raised -tearoff 0]
    wm overrideredirect $data(w:shell) 1
    wm withdraw $data(w:shell)

    set data(w:slistbox) [tixCheckList $data(w:shell).slistbox -scrollbarspace y ]

    set data(w:listbox) [$data(w:slistbox) subwidget hlist]

    pack $data(w:slistbox) -expand yes -fill both -padx 2 -pady 2
}


proc tixComboBoxA:SetBindings {w} {
    upvar #0 $w data
    tixChainMethod $w SetBindings
    tixAddBindTag $data(w:arrow) TixComboArrow
    tixBind TixComboArrow <1>  		      "tixComboBoxA:ArrowDown %W"
    tixBind TixComboArrow <ButtonRelease-1>   "tixComboBoxA:ArrowUp   %W"
    set data(state) 0
    return
}

#----------------------------------------------------------------------
#              Cooked events
#----------------------------------------------------------------------
proc tixComboBoxA:ArrowDown {w} {
    set w [winfo parent [winfo parent $w]]
    upvar #0 $w data
#puts "$w [array exists data]"

    if {![string compare $data(-state) disabled]} {
	return
    }
    
    case $data(state) {
	{0} {
	    tixComboBoxA:GoState 1 $w
	}
	{2} {
	    tixComboBoxA:GoState 19 $w
	}
	default {
	    tixComboBoxA:StateError $w
	}
    }
}

proc tixComboBoxA:ArrowUp {w} {
    set w [winfo parent [winfo parent $w]]
    upvar #0 $w data
    
    case $data(state) {
	{1} {
	    tixComboBoxA:GoState 2 $w
	}
	{19} {
	    # data(ignore) was already set in state 19
	    tixComboBoxA:GoState 4 $w
	}
	{5} {
	    tixComboBoxA:GoState 13 $w
	}
	default {
	    tixComboBoxA:StateError $w
	}
    }
}

#----------------------------------------------------------------------
#           General keyboard event

# returns 1 if the combobox is in some special state and the Escape key
# shouldn't be handled by the toplevel bind tag. As a result, when a combobox
# is popped up in a dialog box, Escape will popdown the combo. If the combo
# is not popped up, Escape will invoke the toplevel bindtag (which can
# pop down the dialog box)
#
proc tixComboBoxA:EscKey {w} {
    upvar #0 $w data

    if {![string compare $data(-state) disabled]} {
	return
    }

    case $data(state) {
	{0} {
	    tixComboBoxA:GoState 17 $w
	}
	{2} {
	    tixComboBoxA:GoState 16 $w
	    return 1
	}
	{n0} {
	    tixComboBoxA:GoState n4 $w
	}
	default {
	    # ignore
	    return 1
	}
    }

    return 0
}


#----------------------------------------------------------------------

proc tixComboBoxA:HandleDirKey {w dir} {
    upvar #0 $w data

    if {[tixComboBoxA:CheckListboxSelection $w]} {
	case $dir {
	    "up" {
		tkListboxUpDown $data(w:listbox) -1
		set data(curIndex) [lindex [$data(w:listbox) curselection] 0]
		tixComboBoxA:SetSelectionFromListbox $w
	    }
	    "down" {
		tkListboxUpDown $data(w:listbox)  1
		set data(curIndex) [lindex [$data(w:listbox) curselection] 0]
		tixComboBoxA:SetSelectionFromListbox $w
	    }
	    "pageup" {
		$data(w:listbox) yview scroll -1 pages
	    }
	    "pagedown" {
		$data(w:listbox) yview scroll  1 pages
	    }
	}
    } else {
	# There wasn't good selection in the listbox.
	#
	tixComboBoxA:SetSelectionFromListbox $w
    }
}

proc tixComboBoxA:Invoke {w} {
    upvar #0 $w data

    tixComboBoxA:SetValue $w $data(-selection)
    if {![winfo exists $w]} {
	return
    }

    $data(w:entry) selection from 0
    $data(w:entry) selection to end
    $data(w:entry) icursor end
}

#----------------------------------------------------------------------
#                   MAINTAINING THE -VALUE
#----------------------------------------------------------------------
proc tixComboBoxA:SetValue {w newValue {noUpdate 0} {updateEnt 1}} {
    upvar #0 $w data

    if {$data(-validatecmd) != ""} {
       set data(-value) [tixEvalCmdBinding $w $data(-validatecmd) "" $newValue]
    } else {
	set data(-value) $newValue
    }

    if {! $noUpdate} {
	tixVariable:UpdateVariable $w
    }

    if {$updateEnt} {
	if {!$data(-editable)} {
	    $data(w:entry) delete 0 end
	    $data(w:entry) insert 0 $data(-value)
	}
    }

    if {!$data(-disablecallback) && $data(-command) != ""} {
	if {![info exists data(varInited)]} {
	    set bind(specs) {%V}
	    set bind(%V)    $data(-value)

	    tixEvalCmdBinding $w $data(-command) bind $data(-value)
	    if {![winfo exists $w]} {
		# The user destroyed the window!
		return
	    }
	}
    }

    set data(-selection) $data(-value)
    if {$updateEnt} {
	tixSetEntry $data(w:entry) $data(-value)

	if {$data(-anchor) == "e"} {
	    tixComboBoxA:EntryAlignEnd $w
	}
    }
}


#----------------------------------------------------------------------
#
#			STATE MANIPULATION
#
#----------------------------------------------------------------------
proc tixComboBoxA:GoState-0 {w} {
    upvar #0 $w data

    if {[info exists data(w:root)] && [grab current] == "$data(w:root)"} {
	grab release $w
    }
}

proc tixComboBoxA:GoState-1 {w} {
    upvar #0 $w data

    tixComboBoxA:Popup $w
}

proc tixComboBoxA:GoState-2 {w} {
    upvar #0 $w data

}

proc tixComboBoxA:GoState-3 {w} {
    upvar #0 $w data

    set data(ignore) 1
    tixComboBoxA:Popdown $w
}

proc tixComboBoxA:Cancel {w} {
    upvar #0 $w data
    set l [$data(w:slistbox) getselection]
# puts "$l"    
#    set $data(-value) [join $l "|"]
#puts "$data(-value)"
#    tixSetEntry $data(w:entry) $data(-value)
#    tixSetEntry $data(w:entry) [join $l "|"]
#    focus $data(w:entry)    
     tixComboBoxA:SetValue $w  [join $l "|"]
}
proc tixComboBoxA:GoState-4 {w} {
    upvar #0 $w data

    tixComboBoxA:Ungrab $w
    if {$data(ignore)} {
	tixComboBoxA:Cancel $w
    } else {
	tixComboBoxA:Invoke $w
    }
    tixComboBoxA:GoState 0 $w
}

proc tixComboBoxA:GoState-5 {w x y X Y} {
    upvar #0 $w data

    tixComboBoxA:LbSelect $w $x $y $X $Y
}

proc tixComboBoxA:GoState-6 {w x y X Y} {
    upvar #0 $w data

    tixComboBoxA:Popdown $w

    if {[tixWithinWindow $data(w:shell) $X $Y]} {
	set data(ignore) 0
    } else {
	set data(ignore) 1
    }

    tixComboBoxA:GoState 4 $w
}

proc tixComboBoxA:GoState-7 {w} {
    upvar #0 $w data

    tixComboBoxA:Popdown $w
    set data(ignore) 1
    catch {
	global tkPriv
	if {$tkPriv(afterId) != ""} {
	    tkCancelRepeat
	}
    }

    set data(ignore) 1
    tixComboBoxA:GoState 4 $w
}

proc tixComboBoxA:GoState-9 {w x y X Y} {
    upvar #0 $w data

    catch {
	tkButtonUp $data(w:arrow)
    }
    tixComboBoxA:GoState 5 $w $x $y $X $Y
}

proc tixComboBoxA:GoState-10 {w dir} {
    upvar #0 $w data

    tixComboBoxA:Popup $w
    if {![tixComboBoxA:CheckListboxSelection $w]} {
	# There wasn't good selection in the listbox.
	#
	tixComboBoxA:SetSelectionFromListbox $w
    }

    tixComboBoxA:GoState 2 $w
}

proc tixComboBoxA:GoState-11 {w dir} {
    upvar #0 $w data

    tixComboBoxA:HandleDirKey $w $dir

    tixComboBoxA:GoState 2 $w
}

proc tixComboBoxA:GoState-12 {w} {
    upvar #0 $w data

    catch {
	tkButtonUp $data(w:arrow)
    }

    tixComboBoxA:GoState 2 $w
}

proc tixComboBoxA:GoState-13 {w} {
    upvar #0 $w data

    catch {
	global tkPriv
	if {$tkPriv(afterId) != ""} {
	    tkCancelRepeat
	}
    }
    tixComboBoxA:GoState 2 $w
}

proc tixComboBoxA:GoState-14 {w} {
    upvar #0 $w data

    tixComboBoxA:Invoke $w
    tixComboBoxA:GoState 0 $w
}

proc tixComboBoxA:GoState-15 {w} {
    upvar #0 $w data

    tixComboBoxA:Popdown $w
    set data(ignore) 0
    tixComboBoxA:GoState 4 $w
}

proc tixComboBoxA:GoState-16 {w} {
    upvar #0 $w data

    tixComboBoxA:Popdown $w
    tixComboBoxA:Cancel $w
    set data(ignore) 1
    tixComboBoxA:GoState 4 $w
}

proc tixComboBoxA:GoState-17 {w} {
    upvar #0 $w data

    tixComboBoxA:Cancel $w
    tixComboBoxA:GoState 0 $w
}

proc tixComboBoxA:GoState-19 {w} {
    upvar #0 $w data

    if {"x$data(-selection)" != "x$data(-value)"} {
	set data(ignore) 0
    } else {
	set data(ignore) 1
    }
    tixComboBoxA:Popdown $w
}

#----------------------------------------------------------------------
#                      Non-dropdown states
#----------------------------------------------------------------------
proc tixComboBoxA:GoState-n0 {w} {
    upvar #0 $w data
}

proc tixComboBoxA:GoState-n1 {w} {
    upvar #0 $w data

    tixComboBoxA:Invoke $w
    tixComboBoxA:GoState n0 $w
}

proc tixComboBoxA:GoState-n3 {w dir} {
    upvar #0 $w data

    tixComboBoxA:HandleDirKey $w $dir
    tixComboBoxA:GoState n0 $w
}

proc tixComboBoxA:GoState-n4 {w} {
    upvar #0 $w data

    tixComboBoxA:Cancel $w
    tixComboBoxA:GoState n0 $w
}

proc tixComboBoxA:GoState-n6 {w x y X Y} {
    upvar #0 $w data
    tixComboBoxA:LbSelect $w $x $y $X $Y
}

#----------------------------------------------------------------------
#                      General State Manipulation
#----------------------------------------------------------------------
proc tixComboBoxA:GoState {s w args} {
    upvar #0 $w data

    tixComboBoxA:SetState $w $s
    eval tixComboBoxA:GoState-$s $w $args
}

proc tixComboBoxA:SetState {w s} {
    upvar #0 $w data

#    catch {puts [info level -2]}
#    puts "setting state $data(state) --> $s"
    set data(state) $s
}

proc tixComboBoxA:StateError {w} {
    upvar #0 $w data

#    error "wrong state $data(state)"
}


#--------------------------------------------------
#		Popping up list shell
#--------------------------------------------------

# Popup the listbox and grab
#
#
proc tixComboBoxA:Popup {w} {
    global tcl_platform
    upvar #0 $w data

    if {![winfo ismapped $data(w:root)]} {
	return
    }

    #---------------------------------------------------------------------
    # 				Pop up
    #
    if {$data(-listcmd) != ""} {
	# This option allows the user to fill in the listbox on demand
	#
	tixEvalCmdBinding $w $data(-listcmd)
    }

    # calculate the size
    set  y [winfo rooty $data(w:entry)]
    incr y [winfo height $data(w:entry)]
    incr y 3

    set bd [$data(w:shell) cget -bd]
    set height [expr [winfo reqheight $data(w:slistbox)] + 2*$bd]

    set x1 [winfo rootx $data(w:entry)]
    if {$data(-listwidth) == ""} {
	if {[winfo ismapped $data(w:arrow)]} {
	    set x2  [winfo rootx $data(w:arrow)]
	    if {$x2 >= $x1} {
		incr x2 [winfo width $data(w:arrow)]
		set width  [expr "$x2 - $x1"]
	    } else {
		set width  [winfo width $data(w:entry)]
		set x2 [expr {$x1 + $width}]
	    }
	} else {
	    set width  [winfo width $data(w:entry)]
	    set x2 [expr {$x1 + $width}]
	}
    } else {
	set width $data(-listwidth)
	set x2 [expr {$x1 + $width}]
    }

    set reqwidth [winfo reqwidth $data(w:shell)]
    if {$reqwidth < $width} {
	set reqwidth $width
    } else {
	if {$reqwidth > [expr {$width *3}]} {
	    set reqwidth [expr {$width *3}]
	}
	if {$reqwidth > [winfo vrootwidth .]} {
	    set reqwidth [winfo vrootwidth .]
	}
    }
    set width $reqwidth


    # If the listbox is too far right, pull it back to the left
    #
    set scrwidth [winfo vrootwidth .]
    if {$x2 > $scrwidth} {
	set x1 [expr {$scrwidth - $width}]
    }

    # If the listbox is too far left, pull it back to the right
    #
    if {$x1 < 0} {
	set x1 0
    }

    # If the listbox is below bottom of screen, put it upwards
    #
    set scrheight [winfo vrootheight .]
    set bottom [expr {$y+$height}]
    if {$bottom > $scrheight} {
	set y [expr $y-$height-[winfo height $data(w:entry)]-5]
    }

    # OK , popup the shell
    #
    global tcl_platform

    wm geometry $data(w:shell) $reqwidth\x$height+$x1+$y
    wm deiconify $data(w:shell)
    raise $data(w:shell)
    focus $data(w:entry)
    set data(popped) 1

    # add for safety
    update
    
    tixComboBoxA:Grab $w
}

proc tixComboBoxA:SetCursor {w cursor} {
    upvar #0 $w data

    $w config -cursor $cursor
}

proc tixComboBoxA:Popdown {w} {
    upvar #0 $w data

    wm withdraw $data(w:shell)
    tixComboBoxA:SetCursor $w ""
}

# Grab the server so that user cannot move the windows around
proc tixComboBoxA:Grab {w} {
    upvar #0 $w data
    tixComboBoxA:SetCursor $w arrow
    if {[catch {
	# We catch here because grab may fail under a lot of circumstances
	# Just don't want to break the code ...
	case $data(-grab) {
	    global {
		tixPushGrab -global $data(w:root)
	    }
	    local {
		tixPushGrab $data(w:root)
	    }
	}
    } err]} {
	tixDebug "tixComboBoxA:Grab+: Error grabbing $data(w:root)\n$err"
    }
}

proc tixComboBoxA:Ungrab {w} {
    upvar #0 $w data
    if {[catch {
	catch {
	    case $data(-grab) {
		global {
		    tixPopGrab
		}
		local {
		    tixPopGrab
		}
	    }
	}
    } err]} {
	tixDebug "tixComboBoxA:Grab+: Error grabbing $data(w:root)\n$err"
    }
}

#----------------------------------------------------------------------
#		 Alignment
#----------------------------------------------------------------------
# The following two routines can emulate a "right align mode" for the
# entry in the combo box.

proc tixComboBoxA:EntryAlignEnd {w} {
    upvar #0 $w data
    $data(w:entry) xview end
}


proc tixComboBoxA:Destructor {w} {
    upvar #0 $w data

    tixUnsetMegaWidget $data(w:entry)
    tixVariable:DeleteVariable $w

    # Chain this to the superclass
    #
    tixChainMethod $w Destructor
}


#----------------------------------------------------------------------
#                           CONFIG OPTIONS
#----------------------------------------------------------------------

proc tixComboBoxA:config-state {w value} {
    upvar #0 $w data
    catch {if {"x[$data(w:arrow) cget -state]" == "x$value"} {
	set a 1
    }}
    if {[info exists a]} {
	return
    }

    catch {$data(w:arrow) config -state $value}
    catch {$data(w:tick)  config -state $value}
    catch {$data(w:cross) config -state $value}
    catch {$data(w:slistbox) config -state $value}

    if {![string compare $value normal]} {
	set fg [$data(w:arrow) cget -fg]
	set entryFg $data(entryfg)
	set lbSelFg [lindex [$data(w:listbox) config -selectforeground] 3]
	set lbSelBg [lindex [$data(w:listbox) config -selectbackground] 3]
	set entrySelFg [lindex [$data(w:entry) config -selectforeground] 3]
	set entrySelBg [lindex [$data(w:entry) config -selectbackground] 3]
    } else {
	set fg [$data(w:arrow) cget -disabledforeground]
	set entryFg $data(-disabledforeground) 
	set lbSelFg $entryFg
	set lbSelBg [$data(w:listbox) cget -bg]
	set entrySelFg $entryFg
	set entrySelBg [$data(w:entry) cget -bg]
    }
    if {[string compare $fg ""]} {
	$data(w:label) config -fg $fg
	$data(w:listbox) config -fg $fg -selectforeground $lbSelFg \
	  -selectbackground $lbSelBg
    }
    $data(w:entry) config -fg $entryFg -selectforeground $entrySelFg \
      -selectbackground $entrySelBg

    if {![string compare $value normal]} {
	if {$data(-editable)} {
	    $data(w:entry) config -state normal
	}
        $data(w:entry) config -takefocus 1
    } else {
	if {$data(-editable)} {
	   $data(w:entry) config -state disabled
        }
        $data(w:entry) config -takefocus 0
    }
}

proc tixComboBoxA:config-value {w value} {
    upvar #0 $w data

    tixComboBoxA:SetValue $w $value

    set data(-selection) $value
    set e [split $value "|"]
    foreach a $e {
    	$data(w:slistbox) setstatus $a on
    }
}

proc tixComboBoxA:config-variable {w arg} {
    upvar #0 $w data

    if {[tixVariable:ConfigVariable $w $arg]} {
       # The value of data(-value) is changed if tixVariable:ConfigVariable 
       # returns true
       set data(-selection) $data(-value)
       tixComboBoxA:SetValue $w $data(-value) 1
    }
    catch {
	unset data(varInited)
    }
    set data(-variable) $arg
}


#----------------------------------------------------------------------
#                     WIDGET COMMANDS
#----------------------------------------------------------------------
proc tixComboBoxA:align {w args} {
    upvar #0 $w data

    if {$data(-anchor) == "e"} {
	tixComboBoxA:EntryAlignEnd $w
    }
}

proc tixComboBoxA:addhistory {w value} {
    upvar #0 $w data

    tixComboBoxA:insert $w 0 $value
    # $data(w:listbox) selection clear 0 end

}

proc tixComboBoxA:appendhistory {w value} {
    upvar #0 $w data

    tixComboBoxA:insert $w end $value
    # $data(w:listbox) selection clear 0 end

}

proc tixComboBoxA:insert {w index newitem} {
    upvar #0 $w data

    $data(w:listbox) add $newitem -itemtype imagetext -text $newitem
    $data(w:slistbox) setstatus $newitem off
}


proc tixComboBoxA:invoke {w} {
    tixComboBoxA:Invoke $w
}

proc tixComboBoxA:popdown {w} {
    upvar #0 $w data

    if {$data(-dropdown)} {
	tixComboBoxA:Popdown $w
    }
}
proc dialog_OC2000_CONFIG { w } {
set wd [create_dialog $w 11 0 OC2000_CONFIG 0 0 0 0]
}
# dialog_OC2000_CONFIG end
proc dialog_OC2000_CONFIG_KERNEL { w } {
set wd [create_dialog $w 10 0 OC2000_CONFIG_KERNEL 0 1 0 0]
create_bool $wd.top 2 "INCLUDE_PRIO256_RDY_QUE" -1 1 0
create_bool $wd.top 3 "INCLUDE_SPAN256_CLK_QUE" -1 2 0
create_int $wd.top 4 3 ISR_STACK_SIZE -1 10
create_int $wd.top 5 4 DEFAULT_STACK_SIZE -1 10
set m [create_menu $wd.top 6 5 DEFAULT_INHERITSCHED -1 0]
set m [create_menu $wd.top 7 6 DEFAULT_SCHEDPOLICY -1 0]
create_int $wd.top 8 7 DEFAULT_SCHED_PRIORITY -1 10
set m [create_menu $wd.top 9 8 DEFAULT_FPUSTATE -1 0]
create_int $wd.top 10 9 DEFAULT_THREAD_QUANT -1 10
}
# dialog_OC2000_CONFIG_KERNEL end
proc dialog_OC2000_CONFIG_SCALING { w } {
set wd [create_dialog $w 13 0 OC2000_CONFIG_SCALING 0 11 0 0]
create_bool $wd.top 12 "INCLUDE_SIGNALS" -1 1 0
dialog_INCLUDE_TIMERS $wd.top
dialog_INCLUDE_POSIX_SEMAPHORES $wd.top
create_bool $wd.top 17 "INCLUDE_POSIX_CND" -1 4 0
dialog_INCLUDE_POSIX_MQ $wd.top
create_bool $wd.top 21 "INCLUDE_PIPES" -1 6 0
dialog_INCLUDE_CONSOLE $wd.top
dialog_INCLUDE_FILESYS $wd.top
create_int $wd.top 32 9 STREAM_MAX_CFG -1 10
create_int $wd.top 33 10 OPEN_MAX_CFG -1 10
dialog_VFS_CFG $wd.top
create_bool $wd.top 38 "MEMALLOC_CLEAR" -1 12 0
}
# dialog_OC2000_CONFIG_SCALING end
proc dialog_INCLUDE_TIMERS { w } {
set wd [create_dialog $w 2 0 INCLUDE_TIMERS 1 13 1 0]
create_int $wd.top 14 1 TIMER_MAX_CFG -1 10
}
# dialog_INCLUDE_TIMERS end
proc dialog_INCLUDE_POSIX_SEMAPHORES { w } {
set wd [create_dialog $w 2 0 INCLUDE_POSIX_SEMAPHORES 1 15 1 0]
create_int $wd.top 16 1 SEM_NSEMS_MAX_CFG -1 10
}
# dialog_INCLUDE_POSIX_SEMAPHORES end
proc dialog_INCLUDE_POSIX_MQ { w } {
set wd [create_dialog $w 3 0 INCLUDE_POSIX_MQ 1 18 1 0]
create_bool $wd.top 19 "INCLUDE_PRIO32_MQ_QUE" -1 1 0
create_int $wd.top 20 2 MQ_OPEN_MAX_CFG -1 10
}
# dialog_INCLUDE_POSIX_MQ end
proc dialog_INCLUDE_CONSOLE { w } {
set wd [create_dialog $w 2 0 INCLUDE_CONSOLE 1 22 1 0]
create_string $wd.top 23 1 CONSOLE_DEVICE -1
}
# dialog_INCLUDE_CONSOLE end
proc dialog_INCLUDE_FILESYS { w } {
set wd [create_dialog $w 8 0 INCLUDE_FILESYS 0 24 1 2]
create_bool $wd.top 25 "INCLUDE_VFAT" -1 1 2
create_bool $wd.top 26 "INCLUDE_CD9660" -1 2 2
create_bool $wd.top 27 "INCLUDE_FTP_FS" -1 3 2
create_bool $wd.top 28 "INCLUDE_TAR_FS" -1 4 2
create_bool $wd.top 29 "INCLUDE_NFS" -1 5 2
create_bool $wd.top 30 "INCLUDE_ENVIRONMENT" -1 6 2
create_bool $wd.top 31 "INCLUDE_IEEE754" -1 7 2
}
# dialog_INCLUDE_FILESYS end
proc dialog_VFS_CFG { w } {
set wd [create_dialog $w 4 0 VFS_CFG 0 34 1 0]
create_int $wd.top 35 1 VFS_VNODES_MAX -1 10
create_int $wd.top 36 2 VFS_BUFFERS_MAX -1 10
create_int $wd.top 37 3 VFS_MALLOC_MAX -1 10
}
# dialog_VFS_CFG end
proc dialog_OC2000_CONFIG_A { w } {
set wd [create_dialog $w 7 0 OC2000_CONFIG_A 0 39 0 0]
create_bool $wd.top 40 "INCLUDE_CTYPE" -1 1 0
create_bool $wd.top 41 "INCLUDE_MATH" -1 2 0
create_bool $wd.top 42 "INCLUDE_STDIO" -1 3 0
create_bool $wd.top 43 "INCLUDE_STDLIB" -1 4 0
create_bool $wd.top 44 "INCLUDE_STRING" -1 5 0
create_bool $wd.top 45 "INCLUDE_TIME" -1 6 0
}
# dialog_OC2000_CONFIG_A end
proc dialog_INCLUDE_NETWORK { w } {
set wd [create_dialog $w 16 0 INCLUDE_NETWORK 1 46 0 0]
create_int $wd.top 47 1 NET_THREAD_PRIORITY -1 10
create_bool $wd.top 48 "INCLUDE_LOCAL_DOMAIN" -1 2 0
create_bool $wd.top 49 "INCLUDE_PING" -1 3 0
create_bool $wd.top 50 "BOOTER_INHERITANCE" -1 4 0
create_bool $wd.top 51 "INCLUDE_TELNET" -1 5 0
dialog_INCLUDE_TELNETD $wd.top
dialog_INCLUDE_RPC $wd.top
dialog_INCLUDE_NETTIME $wd.top
}
# dialog_INCLUDE_NETWORK end
proc dialog_INCLUDE_TELNETD { w } {
set wd [create_dialog $w 2 0 INCLUDE_TELNETD 1 52 1 0]
create_int $wd.top 53 1 TELNETD_PRIORITY -1 10
}
# dialog_INCLUDE_TELNETD end
proc dialog_INCLUDE_RPC { w } {
set wd [create_dialog $w 2 0 INCLUDE_RPC 1 54 1 0]
create_int $wd.top 55 1 PORTMAPD_PRIORITY -1 10
}
# dialog_INCLUDE_RPC end
proc dialog_INCLUDE_NETTIME { w } {
set wd [create_dialog $w 4 0 INCLUDE_NETTIME 1 56 1 0]
create_string $wd.top 57 1 NETTIME_HOST_ADDRESS -1
create_int $wd.top 58 2 NETTIME_ATTEMPT_COUNT -1 10
create_int $wd.top 59 3 NETTIME_TIMEOUT -1 10
}
# dialog_INCLUDE_NETTIME end
proc dialog_CONFIG_NETADDRESSES { w } {
set wd [create_dialog $w 3 0 CONFIG_NETADDRESSES 1 60 0 0]
dialog_INCLUDE_DEFAULT_GATEWAY $wd.top
dialog_INCLUDE_NETADDRESS $wd.top
}
# dialog_CONFIG_NETADDRESSES end
proc dialog_INCLUDE_DEFAULT_GATEWAY { w } {
set wd [create_dialog $w 2 0 INCLUDE_DEFAULT_GATEWAY 1 61 1 0]
create_string $wd.top 62 1 DEFAULT_GATEWAY_ADDRESS -1
}
# dialog_INCLUDE_DEFAULT_GATEWAY end
proc dialog_INCLUDE_NETADDRESS { w } {
global current_line thl; set current_line 0 
set wd [create_dialog $w 5 6 INCLUDE_NETADDRESS 0 63 1 0]
set m [create_menu $wd.top 64 1 _IFNAME 0 1]
create_string $wd.top 65 2 _ADDRESS 1
create_string $wd.top 66 3 _NETMASK 2
create_string $wd.top 67 4 _PTP 3
for {set i 0} { $i < 6 } { incr i } {
checkbutton $thl.b$i -variable INCLUDE_NETADDRESS$i
$thl add $i -itemtype window -window $thl.b$i
set ::arrvars {}
$thl item create  $i 1 -text [set ::_IFNAME$i] 
lappend ::arrvars _IFNAME
$thl item create  $i 2 -text [set ::_ADDRESS$i] 
lappend ::arrvars _ADDRESS
$thl item create  $i 3 -text [set ::_NETMASK$i] 
lappend ::arrvars _NETMASK
$thl item create  $i 4 -text [set ::_PTP$i] 
lappend ::arrvars _PTP
}
$thl selection set 0
bc $wd 0
}
# dialog_INCLUDE_NETADDRESS end
proc dialog_INCLUDE_HOST_TABLE_INIT { w } {
set wd [create_dialog $w 7 0 INCLUDE_HOST_TABLE_INIT 1 68 0 0]
create_int $wd.top 69 1 HOST_TABLE_NODES_COUNT -1 10
create_int $wd.top 70 2 HOST_TABLE_ALIAS_COUNT -1 10
create_int $wd.top 71 3 HOST_TABLE_ADDRESS_COUNT -1 10
create_int $wd.top 72 4 HOST_TABLE_NAME_LENGTH -1 10
create_int $wd.top 73 5 HOST_TABLE_ADDRESS_LENGTH -1 10
dialog_INCLUDE_ADDHOSTS $wd.top
}
# dialog_INCLUDE_HOST_TABLE_INIT end
proc dialog_INCLUDE_ADDHOSTS { w } {
global current_line thl; set current_line 0 
set wd [create_dialog $w 3 5 INCLUDE_ADDHOSTS 0 74 1 0]
create_string $wd.top 75 1 ADDHOSTS_NAME 0
create_string $wd.top 76 2 ADDHOSTS_ADDRESS 1
for {set i 0} { $i < 5 } { incr i } {
checkbutton $thl.b$i -variable INCLUDE_ADDHOSTS$i
$thl add $i -itemtype window -window $thl.b$i
set ::arrvars {}
$thl item create  $i 1 -text [set ::ADDHOSTS_NAME$i] 
lappend ::arrvars ADDHOSTS_NAME
$thl item create  $i 2 -text [set ::ADDHOSTS_ADDRESS$i] 
lappend ::arrvars ADDHOSTS_ADDRESS
}
$thl selection set 0
bc $wd 0
}
# dialog_INCLUDE_ADDHOSTS end
proc dialog_INCLUDE_NETGW { w } {
global current_line thl; set current_line 0 
set wd [create_dialog $w 4 6 INCLUDE_NETGW 1 77 0 0]
create_string $wd.top 78 1 GATEWAY_DST 0
create_string $wd.top 79 2 GATEWAY_GW 1
create_string $wd.top 80 3 GATEWAY_MASK 2
for {set i 0} { $i < 6 } { incr i } {
checkbutton $thl.b$i -variable INCLUDE_NETGW$i
$thl add $i -itemtype window -window $thl.b$i
set ::arrvars {}
$thl item create  $i 1 -text [set ::GATEWAY_DST$i] 
lappend ::arrvars GATEWAY_DST
$thl item create  $i 2 -text [set ::GATEWAY_GW$i] 
lappend ::arrvars GATEWAY_GW
$thl item create  $i 3 -text [set ::GATEWAY_MASK$i] 
lappend ::arrvars GATEWAY_MASK
}
$thl selection set 0
bc $wd 0
}
# dialog_INCLUDE_NETGW end
proc dialog_INCLUDE_SNETGW { w } {
global current_line thl; set current_line 0 
set wd [create_dialog $w 3 6 INCLUDE_SNETGW 1 81 0 0]
create_string $wd.top 82 1 GATEWAY_SDST 0
create_string $wd.top 83 2 GATEWAY_SGW 1
for {set i 0} { $i < 6 } { incr i } {
checkbutton $thl.b$i -variable INCLUDE_SNETGW$i
$thl add $i -itemtype window -window $thl.b$i
set ::arrvars {}
$thl item create  $i 1 -text [set ::GATEWAY_SDST$i] 
lappend ::arrvars GATEWAY_SDST
$thl item create  $i 2 -text [set ::GATEWAY_SGW$i] 
lappend ::arrvars GATEWAY_SGW
}
$thl selection set 0
bc $wd 0
}
# dialog_INCLUDE_SNETGW end
proc dialog_INCLUDE_SLIP { w } {
global current_line thl; set current_line 0 
set wd [create_dialog $w 3 4 INCLUDE_SLIP 1 84 0 0]
create_string $wd.top 85 1 SLIP_DEVNAME 0
create_int $wd.top 86 2 SLIP_MTU 1 10
for {set i 0} { $i < 4 } { incr i } {
checkbutton $thl.b$i -variable INCLUDE_SLIP$i
$thl add $i -itemtype window -window $thl.b$i
set ::arrvars {}
$thl item create  $i 1 -text [set ::SLIP_DEVNAME$i] 
lappend ::arrvars SLIP_DEVNAME
$thl item create  $i 2 -text [set ::SLIP_MTU$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars SLIP_MTU
}
$thl selection set 0
bc $wd 0
}
# dialog_INCLUDE_SLIP end
proc dialog_INCLUDE_PPP { w } {
global current_line thl; set current_line 0 
set wd [create_dialog $w 2 4 INCLUDE_PPP 1 87 0 0]
create_string $wd.top 88 1 PPP_DEVNAME 0
for {set i 0} { $i < 4 } { incr i } {
checkbutton $thl.b$i -variable INCLUDE_PPP$i
$thl add $i -itemtype window -window $thl.b$i
set ::arrvars {}
$thl item create  $i 1 -text [set ::PPP_DEVNAME$i] 
lappend ::arrvars PPP_DEVNAME
}
$thl selection set 0
bc $wd 0
}
# dialog_INCLUDE_PPP end
proc dialog_MBUF_CONFIG { w } {
set wd [create_dialog $w 7 0 MBUF_CONFIG 0 89 0 0]
create_int $wd.top 90 1 NMB_INIT -1 10
create_int $wd.top 91 2 NCL_INIT -1 10
create_int $wd.top 92 3 NMB_ADD -1 10
create_int $wd.top 93 4 NCL_ADD -1 10
create_int $wd.top 94 5 NMB_MAX -1 10
create_int $wd.top 95 6 NCL_MAX -1 10
}
# dialog_MBUF_CONFIG end
proc dialog_INCLUDE_EXTRA_FS { w } {
set wd [create_dialog $w 7 0 INCLUDE_EXTRA_FS 0 96 0 0]
}
# dialog_INCLUDE_EXTRA_FS end
proc dialog_MOUNT_VFAT { w } {
set wd [create_dialog $w 3 0 MOUNT_VFAT 1 97 0 0]
dialog_VFAT_CHECK $wd.top
dialog_VFAT_MNT_VOL $wd.top
}
# dialog_MOUNT_VFAT end
proc dialog_VFAT_CHECK { w } {
set wd [create_dialog $w 6 0 VFAT_CHECK 0 98 1 0]
set m [create_menu $wd.top 99 1 BFSC_OUT -1 0]
set m [create_menu $wd.top 100 2 BFSC_MODE -1 0]
create_bool $wd.top 101 "BFSC_SAVE_BAD" -1 3 0
create_bool $wd.top 102 "BFSC_DO_CHECK_LEN" -1 4 0
create_bool $wd.top 103 "BFSC_DO_SYNC_FATS" -1 5 0
}
# dialog_VFAT_CHECK end
proc dialog_VFAT_MNT_VOL { w } {
global current_line thl; set current_line 0 
set wd [create_dialog $w 9 4 VFAT_MNT_VOL 0 104 1 0]
create_string $wd.top 105 1 VFAT_MOUNTPOINT 0
create_string $wd.top 106 2 VFAT_BDNAME 1
create_bool $wd.top 107 "VFAT_RDONLY" 2 3 0
create_bool $wd.top 108 "VFAT_SYNCHRONOUS" 3 4 0
create_bool $wd.top 109 "VFAT_NOCHECK" 4 5 0
create_int $wd.top 110 6 VFAT_UID 5 10
create_int $wd.top 111 7 VFAT_GID 6 10
create_int $wd.top 112 8 VFAT_ACCESS 7 10
for {set i 0} { $i < 4 } { incr i } {
checkbutton $thl.b$i -variable VFAT_MNT_VOL$i
$thl add $i -itemtype window -window $thl.b$i
set ::arrvars {}
$thl item create  $i 1 -text [set ::VFAT_MOUNTPOINT$i] 
lappend ::arrvars VFAT_MOUNTPOINT
$thl item create  $i 2 -text [set ::VFAT_BDNAME$i] 
lappend ::arrvars VFAT_BDNAME
$thl item create  $i 3 -itemtype imagetext -style [tixDisplayStyle imagetext -anchor e] -image [lindex  $::ck_list [set ::VFAT_RDONLY$i]]
lappend ::arrvars VFAT_RDONLY
$thl item create  $i 4 -itemtype imagetext -style [tixDisplayStyle imagetext -anchor e] -image [lindex  $::ck_list [set ::VFAT_SYNCHRONOUS$i]]
lappend ::arrvars VFAT_SYNCHRONOUS
$thl item create  $i 5 -itemtype imagetext -style [tixDisplayStyle imagetext -anchor e] -image [lindex  $::ck_list [set ::VFAT_NOCHECK$i]]
lappend ::arrvars VFAT_NOCHECK
$thl item create  $i 6 -text [set ::VFAT_UID$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars VFAT_UID
$thl item create  $i 7 -text [set ::VFAT_GID$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars VFAT_GID
$thl item create  $i 8 -text [set ::VFAT_ACCESS$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars VFAT_ACCESS
}
$thl selection set 0
bc $wd 0
}
# dialog_VFAT_MNT_VOL end
proc dialog_MOUNT_CD9660 { w } {
global current_line thl; set current_line 0 
set wd [create_dialog $w 3 2 MOUNT_CD9660 1 113 0 0]
create_string $wd.top 114 1 CD9660_MOUNTPOINT 0
create_string $wd.top 115 2 CD9660_BDNAME 1
for {set i 0} { $i < 2 } { incr i } {
checkbutton $thl.b$i -variable MOUNT_CD9660$i
$thl add $i -itemtype window -window $thl.b$i
set ::arrvars {}
$thl item create  $i 1 -text [set ::CD9660_MOUNTPOINT$i] 
lappend ::arrvars CD9660_MOUNTPOINT
$thl item create  $i 2 -text [set ::CD9660_BDNAME$i] 
lappend ::arrvars CD9660_BDNAME
}
$thl selection set 0
bc $wd 0
}
# dialog_MOUNT_CD9660 end
proc dialog_MOUNT_TAR { w } {
global current_line thl; set current_line 0 
set wd [create_dialog $w 5 4 MOUNT_TAR 1 116 0 0]
create_string $wd.top 117 1 TAR_MOUNTPOINT 0
create_int $wd.top 118 2 TAR_MEMADDRESS 1 10
create_int $wd.top 119 3 TAR_MEMSIZE 2 10
create_bool $wd.top 120 "TAR_CHECK" 3 4 0
for {set i 0} { $i < 4 } { incr i } {
checkbutton $thl.b$i -variable MOUNT_TAR$i
$thl add $i -itemtype window -window $thl.b$i
set ::arrvars {}
$thl item create  $i 1 -text [set ::TAR_MOUNTPOINT$i] 
lappend ::arrvars TAR_MOUNTPOINT
$thl item create  $i 2 -text [set ::TAR_MEMADDRESS$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars TAR_MEMADDRESS
$thl item create  $i 3 -text [set ::TAR_MEMSIZE$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars TAR_MEMSIZE
$thl item create  $i 4 -itemtype imagetext -style [tixDisplayStyle imagetext -anchor e] -image [lindex  $::ck_list [set ::TAR_CHECK$i]]
lappend ::arrvars TAR_CHECK
}
$thl selection set 0
bc $wd 0
}
# dialog_MOUNT_TAR end
proc dialog_INCLUDE_TARFILES { w } {
global current_line thl; set current_line 0 
set wd [create_dialog $w 4 4 INCLUDE_TARFILES 1 121 0 0]
create_string $wd.top 122 1 TAR_PNAME 0
create_fdialog $wd.top 123 2 TAR_FNAME {"*.tar" 1 0} 1
create_bool $wd.top 124 "TAR_FCHECK" 2 3 0
for {set i 0} { $i < 4 } { incr i } {
checkbutton $thl.b$i -variable INCLUDE_TARFILES$i
$thl add $i -itemtype window -window $thl.b$i
set ::arrvars {}
$thl item create  $i 1 -text [set ::TAR_PNAME$i] 
lappend ::arrvars TAR_PNAME
$thl item create  $i 2 -text [set ::TAR_FNAME$i] 
lappend ::arrvars TAR_FNAME
$thl item create  $i 3 -itemtype imagetext -style [tixDisplayStyle imagetext -anchor e] -image [lindex  $::ck_list [set ::TAR_FCHECK$i]]
lappend ::arrvars TAR_FCHECK
}
$thl selection set 0
bc $wd 0
}
# dialog_INCLUDE_TARFILES end
proc dialog_MOUNT_NFS_AUTO { w } {
set wd [create_dialog $w 4 0 MOUNT_NFS_AUTO 1 125 0 0]
create_int $wd.top 126 1 UID -1 10
create_int $wd.top 127 2 GID -1 10
dialog_MOUNT_NFS $wd.top
}
# dialog_MOUNT_NFS_AUTO end
proc dialog_MOUNT_NFS { w } {
global current_line thl; set current_line 0 
set wd [create_dialog $w 7 4 MOUNT_NFS 0 128 1 0]
create_string $wd.top 129 1 NFS_HOST_ADDRESS 0
create_string $wd.top 130 2 NFS_EXPORT 1
create_string $wd.top 131 3 NFS_MOUNTPOINT 2
create_int $wd.top 132 4 NFS_RSIZE 3 10
create_int $wd.top 133 5 NFS_WSIZE 4 10
create_bool $wd.top 134 "NFS_FORCE" 5 6 0
for {set i 0} { $i < 4 } { incr i } {
checkbutton $thl.b$i -variable MOUNT_NFS$i
$thl add $i -itemtype window -window $thl.b$i
set ::arrvars {}
$thl item create  $i 1 -text [set ::NFS_HOST_ADDRESS$i] 
lappend ::arrvars NFS_HOST_ADDRESS
$thl item create  $i 2 -text [set ::NFS_EXPORT$i] 
lappend ::arrvars NFS_EXPORT
$thl item create  $i 3 -text [set ::NFS_MOUNTPOINT$i] 
lappend ::arrvars NFS_MOUNTPOINT
$thl item create  $i 4 -text [set ::NFS_RSIZE$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars NFS_RSIZE
$thl item create  $i 5 -text [set ::NFS_WSIZE$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars NFS_WSIZE
$thl item create  $i 6 -itemtype imagetext -style [tixDisplayStyle imagetext -anchor e] -image [lindex  $::ck_list [set ::NFS_FORCE$i]]
lappend ::arrvars NFS_FORCE
}
$thl selection set 0
bc $wd 0
}
# dialog_MOUNT_NFS end
proc dialog_MOUNT_FTP { w } {
global current_line thl; set current_line 0 
set wd [create_dialog $w 3 3 MOUNT_FTP 1 135 0 0]
create_string $wd.top 136 1 FTP_MOUNTPOINT 0
create_string $wd.top 137 2 FTP_PARSTRING 1
for {set i 0} { $i < 3 } { incr i } {
checkbutton $thl.b$i -variable MOUNT_FTP$i
$thl add $i -itemtype window -window $thl.b$i
set ::arrvars {}
$thl item create  $i 1 -text [set ::FTP_MOUNTPOINT$i] 
lappend ::arrvars FTP_MOUNTPOINT
$thl item create  $i 2 -text [set ::FTP_PARSTRING$i] 
lappend ::arrvars FTP_PARSTRING
}
$thl selection set 0
bc $wd 0
}
# dialog_MOUNT_FTP end
proc dialog_INCLUDE_MULTIPROCESSING { w } {
set wd [create_dialog $w 4 0 INCLUDE_MULTIPROCESSING 1 138 0 0]
dialog_INCLUDE_MP_OBJS $wd.top
dialog_MP_NETWORK $wd.top
}
# dialog_INCLUDE_MULTIPROCESSING end
proc dialog_INCLUDE_MP_OBJS { w } {
set wd [create_dialog $w 10 0 INCLUDE_MP_OBJS 1 139 1 0]
create_int $wd.top 140 1 MASTER_CPU -1 10
create_int $wd.top 141 2 CPU_QUANTITY -1 10
create_int $wd.top 142 3 MP_TCB_MAX -1 10
create_int $wd.top 143 4 MP_SEM_MAX -1 10
create_int $wd.top 144 5 MP_MQP_MAX -1 10
create_int $wd.top 145 6 MP_DIC_MAX -1 10
create_bool $wd.top 146 "INCLUDE_MPMEMPOOL_SELF" -1 7 0
create_int $wd.top 147 8 MP_LOCKTRIES_MAX -1 10
create_bool $wd.top 148 "INCLUDE_MPSHM" -1 9 0
}
# dialog_INCLUDE_MP_OBJS end
proc dialog_MP_NETWORK { w } {
set wd [create_dialog $w 3 0 MP_NETWORK 1 149 1 0]
set m [create_menu $wd.top 150 1 MP_NETWORK_TYPE -1 0]
dialog_VITA_CONFIGURATION $wd.top
}
# dialog_MP_NETWORK end
proc dialog_VITA_CONFIGURATION { w } {
set wd [create_dialog $w 8 0 VITA_CONFIGURATION 0 151 1 0]
create_int $wd.top 152 1 BN_MASTER_ADDR -1 10
create_int $wd.top 153 2 BN_PACKET_SIZE -1 10
create_int $wd.top 154 3 BN_P_PROBE_STARTDELAY -1 10
create_int $wd.top 155 4 BN_P_PROBE_MININTERVAL -1 10
create_int $wd.top 156 5 BN_P_PROBE_MAXCOUNT -1 10
create_int $wd.top 157 6 BN_P_PROBE_BACKOFFCOUNT -1 10
create_int $wd.top 158 7 BN_P_PROBE_MAXINTERVAL -1 10
}
# dialog_VITA_CONFIGURATION end
proc dialog_INCLUDE_CHANNELS { w } {
set wd [create_dialog $w 11 0 INCLUDE_CHANNELS 1 159 0 0]
create_int $wd.top 160 1 MPCHAN_THREAD_PRIORITY -1 10
create_int $wd.top 161 2 MPCHAN_MAX_NUMBER -1 10
create_bool $wd.top 162 "INCLUDE_MPCHAN_VME" -1 3 0
create_bool $wd.top 163 "INCLUDE_MPCHAN_RIO" -1 4 0
create_bool $wd.top 164 "INCLUDE_MPCHAN_NET" -1 5 0
create_bool $wd.top 165 "MPCHAN_ETIMEDOUT_IGNORE" -1 6 0
dialog_MPCHATHREAD_PERIODIC $wd.top
}
# dialog_INCLUDE_CHANNELS end
proc dialog_MPCHATHREAD_PERIODIC { w } {
set wd [create_dialog $w 2 0 MPCHATHREAD_PERIODIC 1 166 1 0]
create_int $wd.top 167 1 MPCHATHREAD_PERIOD -1 10
}
# dialog_MPCHATHREAD_PERIODIC end
proc dialog_INCLUDE_ARINC_QPORT { w } {
set wd [create_dialog $w 4 0 INCLUDE_ARINC_QPORT 1 168 0 0]
create_int $wd.top 169 1 QPORTLIST_MAX_CFG -1 10
create_int $wd.top 170 2 QPORT_OPEN_MAX_CFG -1 10
dialog_KERNEL_QPORT_CONFIG $wd.top
}
# dialog_INCLUDE_ARINC_QPORT end
proc dialog_KERNEL_QPORT_CONFIG { w } {
global current_line thl; set current_line 0 
set wd [create_dialog $w 5 32 KERNEL_QPORT_CONFIG 0 171 1 0]
create_string $wd.top 172 1 KERNEL_QPORT_NAME 0
set m [create_menu $wd.top 173 2 KERNEL_QPORT_DIRECT 1 0]
create_int $wd.top 174 3 KERNEL_QPORT_BUFNUMBER 2 10
create_int $wd.top 175 4 KERNEL_QPORT_BUFSIZE 3 10
for {set i 0} { $i < 32 } { incr i } {
checkbutton $thl.b$i -variable KERNEL_QPORT_CONFIG$i
$thl add $i -itemtype window -window $thl.b$i
set ::arrvars {}
$thl item create  $i 1 -text [set ::KERNEL_QPORT_NAME$i] 
lappend ::arrvars KERNEL_QPORT_NAME
$thl item create  $i 2 -text [set ::KERNEL_QPORT_DIRECT$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars KERNEL_QPORT_DIRECT
$thl item create  $i 3 -text [set ::KERNEL_QPORT_BUFNUMBER$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars KERNEL_QPORT_BUFNUMBER
$thl item create  $i 4 -text [set ::KERNEL_QPORT_BUFSIZE$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars KERNEL_QPORT_BUFSIZE
}
$thl selection set 0
bc $wd 0
}
# dialog_KERNEL_QPORT_CONFIG end
proc dialog_INCLUDE_ARINC_SPORT { w } {
set wd [create_dialog $w 3 0 INCLUDE_ARINC_SPORT 1 176 0 0]
create_int $wd.top 177 1 SPORT_OPEN_MAX_CFG -1 10
dialog_KERNEL_SPORT_CONFIG $wd.top
}
# dialog_INCLUDE_ARINC_SPORT end
proc dialog_KERNEL_SPORT_CONFIG { w } {
global current_line thl; set current_line 0 
set wd [create_dialog $w 5 32 KERNEL_SPORT_CONFIG 0 178 1 0]
create_string $wd.top 179 1 KERNEL_SPORT_NAME 0
set m [create_menu $wd.top 180 2 KERNEL_SPORT_DIRECT 1 0]
create_int $wd.top 181 3 KERNEL_SPORT_BUFSIZE 2 10
create_int $wd.top 182 4 KERNEL_SPORT_REFRESH 3 10
for {set i 0} { $i < 32 } { incr i } {
checkbutton $thl.b$i -variable KERNEL_SPORT_CONFIG$i
$thl add $i -itemtype window -window $thl.b$i
set ::arrvars {}
$thl item create  $i 1 -text [set ::KERNEL_SPORT_NAME$i] 
lappend ::arrvars KERNEL_SPORT_NAME
$thl item create  $i 2 -text [set ::KERNEL_SPORT_DIRECT$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars KERNEL_SPORT_DIRECT
$thl item create  $i 3 -text [set ::KERNEL_SPORT_BUFSIZE$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars KERNEL_SPORT_BUFSIZE
$thl item create  $i 4 -text [set ::KERNEL_SPORT_REFRESH$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars KERNEL_SPORT_REFRESH
}
$thl selection set 0
bc $wd 0
}
# dialog_KERNEL_SPORT_CONFIG end
proc dialog_CONNECTION_CONFIG { w } {
global current_line thl; set current_line 0 
set wd [create_dialog $w 7 32 CONNECTION_CONFIG 0 183 0 0]
create_int $wd.top 184 1 CONNECTION_CHANNEL_ID 0 10
create_string $wd.top 185 2 CONNECTION_CHANNEL_NAME 1
create_string $wd.top 186 3 CONNECTION_PORT_NAME 2
set m [create_menu $wd.top 187 4 CONNECTION_DIRECT 3 0]
create_int $wd.top 188 5 CONNECTION_CPU 4 10
create_int $wd.top 189 6 CONNECTION_NLINK 5 10
for {set i 0} { $i < 32 } { incr i } {
checkbutton $thl.b$i -variable CONNECTION_CONFIG$i
$thl add $i -itemtype window -window $thl.b$i
set ::arrvars {}
$thl item create  $i 1 -text [set ::CONNECTION_CHANNEL_ID$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars CONNECTION_CHANNEL_ID
$thl item create  $i 2 -text [set ::CONNECTION_CHANNEL_NAME$i] 
lappend ::arrvars CONNECTION_CHANNEL_NAME
$thl item create  $i 3 -text [set ::CONNECTION_PORT_NAME$i] 
lappend ::arrvars CONNECTION_PORT_NAME
$thl item create  $i 4 -text [set ::CONNECTION_DIRECT$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars CONNECTION_DIRECT
$thl item create  $i 5 -text [set ::CONNECTION_CPU$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars CONNECTION_CPU
$thl item create  $i 6 -text [set ::CONNECTION_NLINK$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars CONNECTION_NLINK
}
$thl selection set 0
bc $wd 0
}
# dialog_CONNECTION_CONFIG end
proc dialog_OC2000_MISC { w } {
set wd [create_dialog $w 2 0 OC2000_MISC 0 190 0 0]
create_string $wd.top 191 1 ENVIRONMENT -1
}
# dialog_OC2000_MISC end
proc dialog_OC2000_APPLICATION { w } {
set wd [create_dialog $w 5 0 OC2000_APPLICATION 0 192 0 0]
create_bool $wd.top 193 "INCLUDE_LOGO" -1 1 0
create_bool $wd.top 194 "INCLUDE_DEMO_THREAD" -1 2 0
dialog_INCLUDE_USER_THREAD $wd.top
dialog_MAKEFILE_DEFINITIONS $wd.top
}
# dialog_OC2000_APPLICATION end
proc dialog_INCLUDE_USER_THREAD { w } {
set wd [create_dialog $w 3 0 INCLUDE_USER_THREAD 1 195 1 0]
create_string $wd.top 196 1 USER_THREAD_ENTRY -1
create_int $wd.top 197 2 USER_THREAD_PRIORITY -1 10
}
# dialog_INCLUDE_USER_THREAD end
proc dialog_MAKEFILE_DEFINITIONS { w } {
set wd [create_dialog $w 9 0 MAKEFILE_DEFINITIONS 0 198 1 0]
create_fdialog $wd.top 199 1 USER_INCLUDES {"a" 0 1} -1
create_fdialog $wd.top 200 2 USER_SOURCES {"*.c *.s *.S" 0 0} -1
create_fdialog $wd.top 201 3 USER_OBJECTS {"*.o" 0 0} -1
create_fdialog $wd.top 202 4 USER_LIBRARIES {"*.a" 0 0} -1
create_string $wd.top 203 5 EXTERN_SYMBOLS -1
create_ldialog $wd.top 204 6 ADDED_CFLAGS {{-g -w -G8 -O2 -DUSER_VARIABLE=225}} -1
create_string $wd.top 205 7 LNK_OPTIONS -1
create_int $wd.top 206 8 STARTPOINT -1 10
}
# dialog_MAKEFILE_DEFINITIONS end
proc dialog_OC2000_CONFIG_D { w } {
set wd [create_dialog $w 19 0 OC2000_CONFIG_D 0 207 0 0]
set m [create_menu $wd.top 208 1 DEFAULT_INTMODE -1 0]
create_bool $wd.top 209 "INCLUDE_SYMTBL" -1 2 0
create_bool $wd.top 210 "INCLUDE_LOADER" -1 3 0
dialog_INCLUDE_SHELL $wd.top
dialog_INCLUDE_DEBUG $wd.top
dialog_INCLUDE_RDB $wd.top
create_bool $wd.top 221 "INCLUDE_SHOW_OBJECTS" -1 7 0
create_bool $wd.top 222 "INCLUDE_SHOW_MEMORY" -1 8 0
create_bool $wd.top 223 "INCLUDE_SHOW_NETWORK" -1 9 0
dialog_MEMORY_CHECK $wd.top
create_bool $wd.top 228 "INCLUDE_SHOW_INIT" -1 11 0
create_bool $wd.top 229 "DEBUG_LIBRARY" -1 12 0
create_bool $wd.top 248 "INCLUDE_VALIDATION" -1 14 0
}
# dialog_OC2000_CONFIG_D end
proc dialog_INCLUDE_SHELL { w } {
set wd [create_dialog $w 4 0 INCLUDE_SHELL 1 211 1 0]
create_int $wd.top 212 1 SHELL_PRIORITY -1 10
create_int $wd.top 213 2 SHELL_STACK_SIZE -1 10
create_string $wd.top 214 3 SHELL_STARTUP_SCRIPT -1
}
# dialog_INCLUDE_SHELL end
proc dialog_INCLUDE_DEBUG { w } {
set wd [create_dialog $w 2 0 INCLUDE_DEBUG 1 215 1 0]
create_int $wd.top 216 1 DEBUG_PRIORITY -1 10
}
# dialog_INCLUDE_DEBUG end
proc dialog_INCLUDE_RDB { w } {
set wd [create_dialog $w 4 0 INCLUDE_RDB 1 217 1 0]
create_int $wd.top 218 1 RDBTHREAD_PRIORITY -1 10
create_int $wd.top 219 2 RDBFUNCTION_PRIORITY -1 10
create_int $wd.top 220 3 RDBFUNCTION_STACKSIZE -1 10
}
# dialog_INCLUDE_RDB end
proc dialog_MEMORY_CHECK { w } {
set wd [create_dialog $w 2 0 MEMORY_CHECK 1 224 1 0]
dialog_MEMORY_ERROR_PROCESSIG $wd.top
}
# dialog_MEMORY_CHECK end
proc dialog_MEMORY_ERROR_PROCESSIG { w } {
set wd [create_dialog $w 3 0 MEMORY_ERROR_PROCESSIG 0 225 1 2]
create_bool $wd.top 226 "MEMORY_ERROR_SUSPEND" -1 1 2
create_bool $wd.top 227 "MEMORY_ERROR_LOG" -1 2 2
}
# dialog_MEMORY_ERROR_PROCESSIG end
proc dialog_INCLUDE_SYSLOG { w } {
set wd [create_dialog $w 18 0 INCLUDE_SYSLOG 1 230 0 0]
create_int $wd.top 231 1 SYSLOG_PRIORITY -1 10
create_int $wd.top 232 2 SYSLOG_BUF_SIZE_CFG -1 10
create_int $wd.top 233 3 SYSLOG_BUF_COUNT_CFG -1 10
create_bool $wd.top 234 "NOMESSAGE_HEADER" -1 4 0
set m [create_menu $wd.top 235 5 OUTPUT_MSG_TO -1 0]
create_bool $wd.top 236 "OUTPUT_MSG_CONSOLE" -1 6 0
create_bool $wd.top 237 "MEM_FULL_POLICY" -1 7 0
create_string $wd.top 238 8 NAME_OUTSTREAM -1
create_bool $wd.top 239 "ERASE_MEMORY" -1 9 0
create_string $wd.top 240 10 NAME_FMEMORY -1
create_string $wd.top 241 11 MOVE_MEMORY -1
set m [create_menu $wd.top 242 12 MOVE_TYPE -1 0]
create_int $wd.top 243 13 MOVE_MEMORY_SIZE -1 10
create_string $wd.top 244 14 NAME_PMEMORY -1
create_string $wd.top 245 15 SYSLOG_OPEN_FUNCTION -1
create_string $wd.top 246 16 SYSLOG_OUTPUT_FUNCTION -1
create_string $wd.top 247 17 SYSLOG_CLOSE_FUNCTION -1
}
# dialog_INCLUDE_SYSLOG end
proc dialog_REPORT_ERRORS { w } {
set wd [create_dialog $w 9 0 REPORT_ERRORS 1 249 0 0]
create_bool $wd.top 250 "INCLUDE_REPORT_DISASS" -1 1 0
create_bool $wd.top 251 "INCLUDE_REPORT_BACKTR" -1 2 0
create_bool $wd.top 252 "INCLUDE_REPORT_THREADS" -1 3 0
create_bool $wd.top 253 "INCLUDE_REPORT_FLREGS" -1 4 0
create_bool $wd.top 254 "HM_CHECK_OBJECTS" -1 5 0
create_bool $wd.top 255 "HM_REPORT_USER" -1 6 0
create_string $wd.top 256 7 HM_FUNTION_NAME -1
create_int $wd.top 257 8 SIZE_FPAREA -1 10
}
# dialog_REPORT_ERRORS end
proc dialog_INCLUDE_PRSMEM { w } {
set wd [create_dialog $w 4 0 INCLUDE_PRSMEM 1 258 0 0]
set m [create_menu $wd.top 259 1 PRSMEM_ALLOC -1 0]
create_int $wd.top 260 2 PRSMEM_ADDRESS -1 10
dialog_PRSMEM_PART_CONFIG $wd.top
}
# dialog_INCLUDE_PRSMEM end
proc dialog_PRSMEM_PART_CONFIG { w } {
global current_line thl; set current_line 0 
set wd [create_dialog $w 3 8 PRSMEM_PART_CONFIG 0 261 1 0]
create_string $wd.top 262 1 PRSMEM_PART_NAME 0
create_int $wd.top 263 2 PRSMEM_PART_SIZE 1 10
for {set i 0} { $i < 8 } { incr i } {
checkbutton $thl.b$i -variable PRSMEM_PART_CONFIG$i
$thl add $i -itemtype window -window $thl.b$i
set ::arrvars {}
$thl item create  $i 1 -text [set ::PRSMEM_PART_NAME$i] 
lappend ::arrvars PRSMEM_PART_NAME
$thl item create  $i 2 -text [set ::PRSMEM_PART_SIZE$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars PRSMEM_PART_SIZE
}
$thl selection set 0
bc $wd 0
}
# dialog_PRSMEM_PART_CONFIG end
proc dialog_INCLUDE_FLASHMEM { w } {
set wd [create_dialog $w 2 0 INCLUDE_FLASHMEM 1 264 0 0]
dialog_FLASHMEM_PART_CONFIG $wd.top
}
# dialog_INCLUDE_FLASHMEM end
proc dialog_FLASHMEM_PART_CONFIG { w } {
global current_line thl; set current_line 0 
set wd [create_dialog $w 4 8 FLASHMEM_PART_CONFIG 0 265 1 0]
create_string $wd.top 266 1 FLASHMEM_PART_NAME 0
create_int $wd.top 267 2 FLASHMEM_PART_OFFSET 1 10
create_int $wd.top 268 3 FLASHMEM_PART_SIZE 2 10
for {set i 0} { $i < 8 } { incr i } {
checkbutton $thl.b$i -variable FLASHMEM_PART_CONFIG$i
$thl add $i -itemtype window -window $thl.b$i
set ::arrvars {}
$thl item create  $i 1 -text [set ::FLASHMEM_PART_NAME$i] 
lappend ::arrvars FLASHMEM_PART_NAME
$thl item create  $i 2 -text [set ::FLASHMEM_PART_OFFSET$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars FLASHMEM_PART_OFFSET
$thl item create  $i 3 -text [set ::FLASHMEM_PART_SIZE$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars FLASHMEM_PART_SIZE
}
$thl selection set 0
bc $wd 0
}
# dialog_FLASHMEM_PART_CONFIG end
proc dialog_INCLUDE_SYSSHELL { w } {
set wd [create_dialog $w 6 0 INCLUDE_SYSSHELL 1 269 0 0]
create_int $wd.top 270 1 SYSSHELL_PRIORITY -1 10
create_int $wd.top 271 2 SYSTEM_BUF_SIZE -1 10
create_int $wd.top 272 3 SYSTEM_BUF_COUNT -1 10
create_bool $wd.top 273 "SYSTEM_TO_SYSLOG" -1 4 0
create_bool $wd.top 274 "SYSTEM_PRINT" -1 5 0
}
# dialog_INCLUDE_SYSSHELL end
proc dialog_INCLUDE_TRACE { w } {
set wd [create_dialog $w 15 0 INCLUDE_TRACE 1 275 0 0]
create_string $wd.top 276 1 TRACE_NAME -1
create_bool $wd.top 277 "TRACE_CREATE" -1 2 0
create_bool $wd.top 278 "TRACE_AUTOSTART" -1 3 0
dialog_INCLUDE_TRACE_STREAM $wd.top
create_int $wd.top 281 5 TRACE_BLOCKS_SERVICE -1 10
dialog_INCLUDE_TRACE_LOG $wd.top
create_string $wd.top 287 7 TRACE_LOG -1
create_int $wd.top 288 8 TRACE_LOGROTATE -1 10
create_int $wd.top 289 9 TRACE_MAXDATASIZE -1 10
dialog_TRACE_APPEND1 $wd.top
}
# dialog_INCLUDE_TRACE end
proc dialog_INCLUDE_TRACE_STREAM { w } {
set wd [create_dialog $w 2 0 INCLUDE_TRACE_STREAM 0 279 1 0]
set m [create_menu $wd.top 280 1 TRACE_STREAMFULLPOLICY -1 0]
}
# dialog_INCLUDE_TRACE_STREAM end
proc dialog_INCLUDE_TRACE_LOG { w } {
set wd [create_dialog $w 5 0 INCLUDE_TRACE_LOG 1 282 1 0]
create_int $wd.top 283 1 TRACE_LOGSIZE -1 10
set m [create_menu $wd.top 284 2 TRACE_LOGFULLPOLICY -1 0]
create_int $wd.top 285 3 TRACE_LOGPRIORITY -1 10
create_int $wd.top 286 4 TRACE_LOGHIGHPRIORITY -1 10
}
# dialog_INCLUDE_TRACE_LOG end
proc dialog_CONFIG_TRACE_EXCEPTION { w } {
set wd [create_dialog $w 4 0 CONFIG_TRACE_EXCEPTION 0 290 0 0]
create_bool $wd.top 291 "TRACE_EXCEPTION_FP" -1 1 0
create_bool $wd.top 292 "TRACE_EXCEPTION_CALLSTACK" -1 2 0
set m [create_menu $wd.top 293 3 TRACE_EXCEPTIONPOLICY -1 0]
}
# dialog_CONFIG_TRACE_EXCEPTION end
proc dialog_TRACE_CHANNEL_CONFIG { w } {
global current_line thl; set current_line 0 
set wd [create_dialog $w 4 8 TRACE_CHANNEL_CONFIG 1 294 0 0]
create_int $wd.top 295 1 TRACE_CHANNEL_ID 0 10
create_int $wd.top 296 2 TRACE_CHANMSG_SIZE 1 10
create_bool $wd.top 297 "TRACE_CHANMSG_HDR" 2 3 0
for {set i 0} { $i < 8 } { incr i } {
checkbutton $thl.b$i -variable TRACE_CHANNEL_CONFIG$i
$thl add $i -itemtype window -window $thl.b$i
set ::arrvars {}
$thl item create  $i 1 -text [set ::TRACE_CHANNEL_ID$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars TRACE_CHANNEL_ID
$thl item create  $i 2 -text [set ::TRACE_CHANMSG_SIZE$i] -style [tixDisplayStyle text -anchor e]
lappend ::arrvars TRACE_CHANMSG_SIZE
$thl item create  $i 3 -itemtype imagetext -style [tixDisplayStyle imagetext -anchor e] -image [lindex  $::ck_list [set ::TRACE_CHANMSG_HDR$i]]
lappend ::arrvars TRACE_CHANMSG_HDR
}
$thl selection set 0
bc $wd 0
}
# dialog_TRACE_CHANNEL_CONFIG end
proc dialog_TRACE_APPEND1 { w } {
set wd [create_dialog $w 2 0 TRACE_APPEND1 0 298 1 0]
create_bool $wd.top 299 "TRACE_BUFFER_SAVE" -1 1 0
}
# dialog_TRACE_APPEND1 end
proc dialog_TRACE_LOAD_CONFIG { w } {
set wd [create_dialog $w 3 0 TRACE_LOAD_CONFIG 1 300 0 0]
create_string $wd.top 301 1 TRACE_CONFIG_FILE -1
create_string $wd.top 302 2 TRACE_CONFIG_MASK_REGISTRATION -1
}
# dialog_TRACE_LOAD_CONFIG end
proc dialog_TRACE_EVENT { w } {
set wd [create_dialog $w 20 0 TRACE_EVENT 1 303 0 0]
dialog_TRACE_GLB_INTERRUPT $wd.top
create_bool $wd.top 307 "TRACE_GLB_EXCEPTION" -1 2 0
dialog_TRACE_GLB_THREADSWITCH $wd.top
dialog_TRACE_GLB_THREADSTATUS $wd.top
dialog_TRACE_GLB_SIGNAL $wd.top
dialog_TRACE_GLB_SIGWAIT $wd.top
dialog_TRACE_GLB_MEMORY $wd.top
dialog_TRACE_GLB_THREAD $wd.top
create_bool $wd.top 346 "TRACE_GLB_RETURN" -1 9 0
dialog_TRACE_GLB_SEMAPHORE $wd.top
dialog_TRACE_GLB_MUTEX $wd.top
dialog_TRACE_GLB_COND $wd.top
dialog_TRACE_GLB_MQP $wd.top
dialog_TRACE_GLB_TMR $wd.top
dialog_TRACE_GLB_IO $wd.top
dialog_TRACE_MPMEM $wd.top
dialog_TRACE_MPSEM $wd.top
dialog_TRACE_MPMQ $wd.top
dialog_TRACE_GLB_USEREVENT $wd.top
}
# dialog_TRACE_EVENT end
proc dialog_TRACE_GLB_INTERRUPT { w } {
set wd [create_dialog $w 3 0 TRACE_GLB_INTERRUPT 1 304 1 2]
create_bool $wd.top 305 "TRACE_INTERRUPT_ENTRY" -1 1 2
create_bool $wd.top 306 "TRACE_INTERRUPT_EXIT" -1 2 2
}
# dialog_TRACE_GLB_INTERRUPT end
proc dialog_TRACE_GLB_THREADSWITCH { w } {
set wd [create_dialog $w 3 0 TRACE_GLB_THREADSWITCH 1 308 1 2]
create_bool $wd.top 309 "TRACE_THREADSWITCH" -1 1 2
create_bool $wd.top 310 "TRACE_THREADIDLE" -1 2 2
}
# dialog_TRACE_GLB_THREADSWITCH end
proc dialog_TRACE_GLB_THREADSTATUS { w } {
set wd [create_dialog $w 9 0 TRACE_GLB_THREADSTATUS 1 311 1 2]
create_bool $wd.top 312 "TRACE_THREADCREATE" -1 1 2
create_bool $wd.top 313 "TRACE_THREADEXIT" -1 2 2
create_bool $wd.top 314 "TRACE_THREADACTIVATE" -1 3 2
create_bool $wd.top 315 "TRACE_THREADBLOCK" -1 4 2
create_bool $wd.top 316 "TRACE_THREADSCHEDYIELD" -1 5 2
create_bool $wd.top 317 "TRACE_THREADTIMEQUANTUM" -1 6 2
create_bool $wd.top 318 "TRACE_THREADSUSPEND" -1 7 2
create_bool $wd.top 319 "TRACE_THREADRESUME" -1 8 2
}
# dialog_TRACE_GLB_THREADSTATUS end
proc dialog_TRACE_GLB_SIGNAL { w } {
set wd [create_dialog $w 6 0 TRACE_GLB_SIGNAL 1 320 1 2]
create_bool $wd.top 321 "TRACE_SIGGENERATION" -1 1 2
create_bool $wd.top 322 "TRACE_SIGDELIVERY" -1 2 2
create_bool $wd.top 323 "TRACE_SIGCATCHFUNC" -1 3 2
create_bool $wd.top 324 "TRACE_SIGCATCHFUNCRET" -1 4 2
create_bool $wd.top 325 "TRACE_SIGTHREADCANCEL" -1 5 2
}
# dialog_TRACE_GLB_SIGNAL end
proc dialog_TRACE_GLB_SIGWAIT { w } {
set wd [create_dialog $w 6 0 TRACE_GLB_SIGWAIT 1 326 1 2]
create_bool $wd.top 327 "TRACE_SIGPAUSE" -1 1 2
create_bool $wd.top 328 "TRACE_SIGSUSPEND" -1 2 2
create_bool $wd.top 329 "TRACE_SIGTIMEDWAIT" -1 3 2
create_bool $wd.top 330 "TRACE_SIGWAIT" -1 4 2
create_bool $wd.top 331 "TRACE_SIGWAITINFO" -1 5 2
}
# dialog_TRACE_GLB_SIGWAIT end
proc dialog_TRACE_GLB_MEMORY { w } {
set wd [create_dialog $w 6 0 TRACE_GLB_MEMORY 1 332 1 2]
create_bool $wd.top 333 "TRACE_MEMPOOLCREATE" -1 1 2
create_bool $wd.top 334 "TRACE_MEMPOOLADD" -1 2 2
create_bool $wd.top 335 "TRACE_MEMPOOLDELETE" -1 3 2
create_bool $wd.top 336 "TRACE_MEMPOOLALLOC" -1 4 2
create_bool $wd.top 337 "TRACE_MEMPOOLFREE" -1 5 2
}
# dialog_TRACE_GLB_MEMORY end
proc dialog_TRACE_GLB_THREAD { w } {
set wd [create_dialog $w 8 0 TRACE_GLB_THREAD 1 338 1 2]
create_bool $wd.top 339 "TRACE_THREADCANCEL" -1 1 2
create_bool $wd.top 340 "TRACE_THREADJOIN" -1 2 2
create_bool $wd.top 341 "TRACE_THREADSIGMASK" -1 3 2
create_bool $wd.top 342 "TRACE_THREADSETSCHEDPARAM" -1 4 2
create_bool $wd.top 343 "TRACE_THREADSETCANCELSTATE" -1 5 2
create_bool $wd.top 344 "TRACE_THREADSETCANCELTYPE" -1 6 2
create_bool $wd.top 345 "TRACE_THREADDETACH" -1 7 2
}
# dialog_TRACE_GLB_THREAD end
proc dialog_TRACE_GLB_SEMAPHORE { w } {
set wd [create_dialog $w 12 0 TRACE_GLB_SEMAPHORE 1 347 1 2]
create_bool $wd.top 348 "TRACE_SEMGETVALUE" -1 1 2
create_bool $wd.top 349 "TRACE_SEMOPEN" -1 2 2
create_bool $wd.top 350 "TRACE_SEMCLOSE" -1 3 2
create_bool $wd.top 351 "TRACE_SEMUNLINK" -1 4 2
create_bool $wd.top 352 "TRACE_SEMINIT" -1 5 2
create_bool $wd.top 353 "TRACE_SEMDESTROY" -1 6 2
create_bool $wd.top 354 "TRACE_SEMPOST" -1 7 2
create_bool $wd.top 355 "TRACE_DELAYEDSEMPOST" -1 8 2
create_bool $wd.top 356 "TRACE_SEMWAIT" -1 9 2
create_bool $wd.top 357 "TRACE_SEMTIMEDWAIT" -1 10 2
create_bool $wd.top 358 "TRACE_SEMTRYWAIT" -1 11 2
}
# dialog_TRACE_GLB_SEMAPHORE end
proc dialog_TRACE_GLB_MUTEX { w } {
set wd [create_dialog $w 9 0 TRACE_GLB_MUTEX 1 359 1 2]
create_bool $wd.top 360 "TRACE_MUTEXDESTROY" -1 1 2
create_bool $wd.top 361 "TRACE_MUTEXINIT" -1 2 2
create_bool $wd.top 362 "TRACE_MUTEXDSETPRIOCEILING" -1 3 2
create_bool $wd.top 363 "TRACE_MUTEXGETPRIOCEILING" -1 4 2
create_bool $wd.top 364 "TRACE_MUTEXLOCK" -1 5 2
create_bool $wd.top 365 "TRACE_MUTEXTRYLOCK" -1 6 2
create_bool $wd.top 366 "TRACE_MUTEXTIMEDLOCK" -1 7 2
create_bool $wd.top 367 "TRACE_MUTEXUNLOCK" -1 8 2
}
# dialog_TRACE_GLB_MUTEX end
proc dialog_TRACE_GLB_COND { w } {
set wd [create_dialog $w 7 0 TRACE_GLB_COND 1 368 1 2]
create_bool $wd.top 369 "TRACE_CONDBROADCAST" -1 1 2
create_bool $wd.top 370 "TRACE_CONDDESTROY" -1 2 2
create_bool $wd.top 371 "TRACE_CONDINIT" -1 3 2
create_bool $wd.top 372 "TRACE_CONDSIGNAL" -1 4 2
create_bool $wd.top 373 "TRACE_CONDTIMEDWAIT" -1 5 2
create_bool $wd.top 374 "TRACE_CONDWAIT" -1 6 2
}
# dialog_TRACE_GLB_COND end
proc dialog_TRACE_GLB_MQP { w } {
set wd [create_dialog $w 11 0 TRACE_GLB_MQP 1 375 1 2]
create_bool $wd.top 376 "TRACE_MQCLOSE" -1 1 2
create_bool $wd.top 377 "TRACE_MQNOTIFY" -1 2 2
create_bool $wd.top 378 "TRACE_MQOPEN" -1 3 2
create_bool $wd.top 379 "TRACE_MQRECEIVE" -1 4 2
create_bool $wd.top 380 "TRACE_MQTIMEDRECEIVE" -1 5 2
create_bool $wd.top 381 "TRACE_MQSEND" -1 6 2
create_bool $wd.top 382 "TRACE_MQTIMEDSEND" -1 7 2
create_bool $wd.top 383 "TRACE_MQSETATTR" -1 8 2
create_bool $wd.top 384 "TRACE_MQGETATTR" -1 9 2
create_bool $wd.top 385 "TRACE_MQUNLINK" -1 10 2
}
# dialog_TRACE_GLB_MQP end
proc dialog_TRACE_GLB_TMR { w } {
set wd [create_dialog $w 9 0 TRACE_GLB_TMR 1 386 1 2]
create_bool $wd.top 387 "TRACE_TMRSLEEP" -1 1 2
create_bool $wd.top 388 "TRACE_TMRNANOSLEEP" -1 2 2
create_bool $wd.top 389 "TRACE_TMREXPIRE" -1 3 2
create_bool $wd.top 390 "TRACE_TMRCREATE" -1 4 2
create_bool $wd.top 391 "TRACE_TMRDELETE" -1 5 2
create_bool $wd.top 392 "TRACE_TMRGETOVERRUN" -1 6 2
create_bool $wd.top 393 "TRACE_TMRSETTIME" -1 7 2
create_bool $wd.top 394 "TRACE_TMRCLCSETTIME" -1 8 2
}
# dialog_TRACE_GLB_TMR end
proc dialog_TRACE_GLB_IO { w } {
set wd [create_dialog $w 18 0 TRACE_GLB_IO 1 395 1 2]
create_bool $wd.top 396 "TRACE_IOMKFIFO" -1 1 2
create_bool $wd.top 397 "TRACE_IOLSEEK" -1 2 2
create_bool $wd.top 398 "TRACE_IOOPEN" -1 3 2
create_bool $wd.top 399 "TRACE_IOCLOSE" -1 4 2
create_bool $wd.top 400 "TRACE_IODUP" -1 5 2
create_bool $wd.top 401 "TRACE_IODUP2" -1 6 2
create_bool $wd.top 402 "TRACE_IOPIPE" -1 7 2
create_bool $wd.top 403 "TRACE_IOREAD" -1 8 2
create_bool $wd.top 404 "TRACE_IOWRITE" -1 9 2
create_bool $wd.top 405 "TRACE_IOFDATASYNC" -1 10 2
create_bool $wd.top 406 "TRACE_AIOREAD" -1 11 2
create_bool $wd.top 407 "TRACE_AIOWRITE" -1 12 2
create_bool $wd.top 408 "TRACE_AIOFSYNC" -1 13 2
create_bool $wd.top 409 "TRACE_AIOCANCEL" -1 14 2
create_bool $wd.top 410 "TRACE_AIOERROR" -1 15 2
create_bool $wd.top 411 "TRACE_AIORETURN" -1 16 2
create_bool $wd.top 412 "TRACE_AIOSUSPEND" -1 17 2
}
# dialog_TRACE_GLB_IO end
proc dialog_TRACE_MPMEM { w } {
set wd [create_dialog $w 5 0 TRACE_MPMEM 1 413 1 2]
create_bool $wd.top 414 "TRACE_MPMEMPOOLCREATE" -1 1 2
create_bool $wd.top 415 "TRACE_MPMEMPOOLADD" -1 2 2
create_bool $wd.top 416 "TRACE_MPMEMPOOLALLOC" -1 3 2
create_bool $wd.top 417 "TRACE_MPMEMPOOLFREE" -1 4 2
}
# dialog_TRACE_MPMEM end
proc dialog_TRACE_MPSEM { w } {
set wd [create_dialog $w 8 0 TRACE_MPSEM 1 418 1 2]
create_bool $wd.top 419 "TRACE_MPSEMTRYWAIT" -1 1 2
create_bool $wd.top 420 "TRACE_MPSEMTIMEDWAIT" -1 2 2
create_bool $wd.top 421 "TRACE_MPSEMWAIT" -1 3 2
create_bool $wd.top 422 "TRACE_MPSEMPOST" -1 4 2
create_bool $wd.top 423 "TRACE_MPSEMINIT" -1 5 2
create_bool $wd.top 424 "TRACE_MPSEMOPEN" -1 6 2
create_bool $wd.top 425 "TRACE_MPSEMGETVALUE" -1 7 2
}
# dialog_TRACE_MPSEM end
proc dialog_TRACE_MPMQ { w } {
set wd [create_dialog $w 8 0 TRACE_MPMQ 1 426 1 2]
create_bool $wd.top 427 "TRACE_MPMQOPEN" -1 1 2
create_bool $wd.top 428 "TRACE_MPMQRECEIVE" -1 2 2
create_bool $wd.top 429 "TRACE_MPMQTIMEDRECEIVE" -1 3 2
create_bool $wd.top 430 "TRACE_MPMQSEND" -1 4 2
create_bool $wd.top 431 "TRACE_MPMQTIMEDSEND" -1 5 2
create_bool $wd.top 432 "TRACE_MPMQSETATTR" -1 6 2
create_bool $wd.top 433 "TRACE_MPMQGETATTR" -1 7 2
}
# dialog_TRACE_MPMQ end
proc dialog_TRACE_GLB_USEREVENT { w } {
set wd [create_dialog $w 2 0 TRACE_GLB_USEREVENT 0 434 1 0]
create_bool $wd.top 435 "TRACE_GLB_USER" -1 1 0
}
# dialog_TRACE_GLB_USEREVENT end
proc dialog_INCLUDE_GTK { w } {
set wd [create_dialog $w 4 0 INCLUDE_GTK 1 436 0 0]
create_fdialog $wd.top 437 1 GTK_DIR {"a" 1 1} -1
create_string $wd.top 438 2 TEXTDOMAINDIR -1
create_string $wd.top 439 3 XLOCALEDIR -1
}
# dialog_INCLUDE_GTK end
proc dialog_XWINDOW_INCLUDE { w } {
set wd [create_dialog $w 5 0 XWINDOW_INCLUDE 0 440 0 0]
create_fdialog $wd.top 441 1 OC2KWINHOME {"a" 1 1} -1
create_fdialog $wd.top 442 2 XSERVERBASE {"a" 1 1} -1
dialog_INCLUDE_XCLIENT $wd.top
dialog_INCLUDE_XSERVER $wd.top
}
# dialog_XWINDOW_INCLUDE end
proc dialog_INCLUDE_XCLIENT { w } {
set wd [create_dialog $w 5 0 INCLUDE_XCLIENT 1 443 1 0]
create_bool $wd.top 444 "INCLUDE_GBRV" -1 1 0
create_bool $wd.top 445 "INCLUDE_BICRV" -1 2 0
create_bool $wd.top 446 "ENVDISPLAY_SET" -1 3 0
create_string $wd.top 447 4 ENVDISPLAY_VALUE -1
}
# dialog_INCLUDE_XCLIENT end
proc dialog_INCLUDE_XSERVER { w } {
set wd [create_dialog $w 16 0 INCLUDE_XSERVER 1 448 1 0]
create_bool $wd.top 449 "XSERVER_START" -1 1 0
create_int $wd.top 450 2 XSERVER_PRIORITY -1 10
set m [create_menu $wd.top 451 3 XSERVER_NAME -1 0]
create_bool $wd.top 452 "XSERVER_HOSTSCNT" -1 4 0
create_bool $wd.top 453 "XSERVER_PARMCONFIG" -1 5 0
create_string $wd.top 454 6 XSERVER_PREFIX -1
create_string $wd.top 455 7 XSERVER_RGB -1
create_string $wd.top 456 8 XSERVER_FONTS -1
create_int $wd.top 457 9 XSERVER_DEPTH -1 10
create_string $wd.top 458 10 XSERVER_MODES -1
dialog_XSERVER_CFGTYPE $wd.top
create_string $wd.top 461 12 XSERVER_CFGFILE -1
create_string $wd.top 462 13 XSERVER_OUT -1
create_bool $wd.top 463 "XSERVER_LOG" -1 14 0
create_string $wd.top 464 15 XSERVER_LOGFILE -1
}
# dialog_INCLUDE_XSERVER end
proc dialog_XSERVER_CFGTYPE { w } {
set wd [create_dialog $w 2 0 XSERVER_CFGTYPE 1 459 1 0]
create_fdialog $wd.top 460 1 XSERVERCFGPATH {"a" 1 1} -1
}
# dialog_XSERVER_CFGTYPE end
#### =================== dialog part end ======== 
#### =================== dialog part end ======== 
#### =================== dialog part end ======== 
#### =================== dialog part end ======== 
#### =================== dialog part end ======== 
set i ""
addvar OC2000_CONFIG$i $i {configos.h} u  "1" 0 {} {} {} {} {} {guide/P_config_dial.htm} {$::env(OC2000DOC)} {} {OS Configuration} OC2000_CONFIG {} 0 bool
addvar OC2000_CONFIG_KERNEL$i $i {configos.h} u  "1" 0 {} {} {} {} {} {guide/P_config_dial_kernel.htm} {} {} {Kernel} OC2000_CONFIG.OC2000_CONFIG_KERNEL {} 0 bool
addvar INCLUDE_PRIO256_RDY_QUE$i $i {configos.h} u  "0" 0 {} {} {} {} {Recommended if queue is big} {} {} {} {Ready queue PRIO256} OC2000_CONFIG.OC2000_CONFIG_KERNEL.INCLUDE_PRIO256_RDY_QUE {} 0 bool
addvar INCLUDE_SPAN256_CLK_QUE$i $i {configos.h} u  "0" 1 {} {} {} {} {Recommended if queue is big} {} {} {} {Timer queue SPAN256} OC2000_CONFIG.OC2000_CONFIG_KERNEL.INCLUDE_SPAN256_CLK_QUE {} 0 bool
addvar ISR_STACK_SIZE$i $i {configos.h} i  "8196" 2 {} {} {} {} {Stack size for interrupt routines} {} {} {} {ISR Stack size} OC2000_CONFIG.OC2000_CONFIG_KERNEL.ISR_STACK_SIZE {} 0 int
addvar DEFAULT_STACK_SIZE$i $i {configos.h} i  "16384" 3 {} {} {} {} {Default stack size for theads} {} {} {} {Default stack size} OC2000_CONFIG.OC2000_CONFIG_KERNEL.DEFAULT_STACK_SIZE {} 0 int
addvar DEFAULT_INHERITSCHED$i $i {configos.h} i  "PTHREAD_EXPLICIT_SCHED" 4 {} {} {} {} {The default value for the inheritsched attribute} {} {} {} {Default inheritsched} OC2000_CONFIG.OC2000_CONFIG_KERNEL.DEFAULT_INHERITSCHED { {PTHREAD_EXPLICIT_SCHED}  {PTHREAD_INHERIT_SCHED} } 0 combo
addvar DEFAULT_SCHEDPOLICY$i $i {configos.h} i  "SCHED_FIFO" 5 {} {} {} {} {The default value for the schedpolicy attribute} {} {} {} {Default schedpolicy} OC2000_CONFIG.OC2000_CONFIG_KERNEL.DEFAULT_SCHEDPOLICY { {SCHED_FIFO}  {SCHED_RR}  {SCHED_OTHER} } 0 combo
addvar DEFAULT_SCHED_PRIORITY$i $i {configos.h} i  "100" 6 {} {} {} {$P >= 0 && $P <= 255} {The default value for thread priority} {} {} {} {Default priority} OC2000_CONFIG.OC2000_CONFIG_KERNEL.DEFAULT_SCHED_PRIORITY {} 0 int
addvar DEFAULT_FPUSTATE$i $i {configos.h} i  "PTHREAD_FPU_ENABLE" 7 {} {} {} {} {The default value for the fpustate attribute} {} {} {} {Default fpustate} OC2000_CONFIG.OC2000_CONFIG_KERNEL.DEFAULT_FPUSTATE { {PTHREAD_FPU_ENABLE}  {PTHREAD_FPU_DISABLE} } 0 combo
addvar DEFAULT_THREAD_QUANT$i $i {configos.h} i  "4" 8 {} {} {} {} {Execution time limit for SCHED_RR scheduling policy (in ticks)} {} {} {} {Time quantum} OC2000_CONFIG.OC2000_CONFIG_KERNEL.DEFAULT_THREAD_QUANT {} 0 int
addvar OC2000_CONFIG_SCALING$i $i {configos.h} u  "1" 1 {} {} {} {} {} {guide/P_config_dial_scaling.htm} {} {} {Scaling} OC2000_CONFIG.OC2000_CONFIG_SCALING {} 0 bool
addvar INCLUDE_SIGNALS$i $i {configos.h} u  "1" 0 {} {} {} {} {Include signal support} {} {} {} {Include signals} OC2000_CONFIG.OC2000_CONFIG_SCALING.INCLUDE_SIGNALS {} 0 bool
addvar INCLUDE_TIMERS$i $i {configos.h} u  "1" 1 {} {INCLUDE_NETWORK } {} {} {Include timer support} {} {} {} {Include timers} OC2000_CONFIG.OC2000_CONFIG_SCALING.INCLUDE_TIMERS {} 0 bool
addvar TIMER_MAX_CFG$i $i {configos.h} i  "10" 0 {} {} {} {} {Maximum number of timers} {} {} {} {Number of timers} OC2000_CONFIG.OC2000_CONFIG_SCALING.INCLUDE_TIMERS.TIMER_MAX_CFG {} 0 int
addvar INCLUDE_POSIX_SEMAPHORES$i $i {configos.h} u  "1" 2 {} {INCLUDE_DEBUG } {} {} {Include semaphore support} {} {} {} {Include semaphores} OC2000_CONFIG.OC2000_CONFIG_SCALING.INCLUDE_POSIX_SEMAPHORES {} 0 bool
addvar SEM_NSEMS_MAX_CFG$i $i {configos.h} i  "140" 0 {} {} {} {} {Maximum number of semaphores} {} {} {} {Number of semaphores} OC2000_CONFIG.OC2000_CONFIG_SCALING.INCLUDE_POSIX_SEMAPHORES.SEM_NSEMS_MAX_CFG {} 0 int
addvar INCLUDE_POSIX_CND$i $i {configos.h} u  "1" 3 {} {INCLUDE_XCLIENT INCLUDE_XSERVER } {} {} {Include conditional variable support} {} {} {} {Include cond. var.} OC2000_CONFIG.OC2000_CONFIG_SCALING.INCLUDE_POSIX_CND {} 0 bool
addvar INCLUDE_POSIX_MQ$i $i {configos.h} u  "1" 4 {} {INCLUDE_NETWORK } {} {} {Include message queue support} {} {} {} {Include msg queues} OC2000_CONFIG.OC2000_CONFIG_SCALING.INCLUDE_POSIX_MQ {} 0 bool
addvar INCLUDE_PRIO32_MQ_QUE$i $i {configos.h} u  "0" 0 {} {} {} {} {Recommended if queue is big} {} {} {} {Message que PRIO32} OC2000_CONFIG.OC2000_CONFIG_SCALING.INCLUDE_POSIX_MQ.INCLUDE_PRIO32_MQ_QUE {} 0 bool
addvar MQ_OPEN_MAX_CFG$i $i {configos.h} i  "10" 1 {} {} {} {} {Maximum number of message queues} {} {} {} {Number of msg queues} OC2000_CONFIG.OC2000_CONFIG_SCALING.INCLUDE_POSIX_MQ.MQ_OPEN_MAX_CFG {} 0 int
addvar INCLUDE_PIPES$i $i {configos.h} u  "1" 5 {} {} {} {} {Include pipes support} {} {} {} {Include pipes} OC2000_CONFIG.OC2000_CONFIG_SCALING.INCLUDE_PIPES {} 0 bool
addvar INCLUDE_CONSOLE$i $i {configos.h} u  "1" 6 {} {} {} {} {Include console support} {} {} {} {Include console} OC2000_CONFIG.OC2000_CONFIG_SCALING.INCLUDE_CONSOLE {} 0 bool
addvar CONSOLE_DEVICE$i $i {configos.h} s  "/dev/console" 0 {} {} {} {} {Console device name} {} {} {} {Console name} OC2000_CONFIG.OC2000_CONFIG_SCALING.INCLUDE_CONSOLE.CONSOLE_DEVICE {} 0 text
addvar INCLUDE_FILESYS$i $i {configos.h} u  "1" 7 {} {} {} {} {} {} {} {} {} OC2000_CONFIG.OC2000_CONFIG_SCALING.INCLUDE_FILESYS {} 0 bool
addvar INCLUDE_VFAT$i $i {configos.h} u  "0" 0 {} {MOUNT_VFAT INCLUDE_TRACE_LOG TRACE_LOG TRACE_LOAD_CONFIG } {} {} {Include VFAT file system support} {} {} {} {Include VFAT} OC2000_CONFIG.OC2000_CONFIG_SCALING.INCLUDE_FILESYS.INCLUDE_VFAT {} 0 bool
addvar INCLUDE_CD9660$i $i {configos.h} u  "0" 1 {} {MOUNT_CD9660 } {} {} {Include CD-ISO9660 file system support} {} {} {} {Include CD-ISO9660} OC2000_CONFIG.OC2000_CONFIG_SCALING.INCLUDE_FILESYS.INCLUDE_CD9660 {} 0 bool
addvar INCLUDE_FTP_FS$i $i {configos.h} u  "0" 2 {$INCLUDE_NETWORK==1} {MOUNT_FTP } {} {} {Include FTP file system support} {} {} {} {Include FTP} OC2000_CONFIG.OC2000_CONFIG_SCALING.INCLUDE_FILESYS.INCLUDE_FTP_FS {} 0 bool
addvar INCLUDE_TAR_FS$i $i {configos.h} u  "0" 3 {} {MOUNT_TAR INCLUDE_TARFILES XSERVER_CFGTYPE } {} {} {Include TAR file system support} {} {} {} {Include TAR} OC2000_CONFIG.OC2000_CONFIG_SCALING.INCLUDE_FILESYS.INCLUDE_TAR_FS {} 0 bool
addvar INCLUDE_NFS$i $i {configos.h} u  "0" 4 {$INCLUDE_NETWORK==1} {MOUNT_NFS_AUTO INCLUDE_TRACE_LOG TRACE_LOG TRACE_LOAD_CONFIG } {} {} {Include NFS file system support} {} {} {} {Include NFS} OC2000_CONFIG.OC2000_CONFIG_SCALING.INCLUDE_FILESYS.INCLUDE_NFS {} 0 bool
addvar INCLUDE_ENVIRONMENT$i $i {configos.h} u  "1" 5 {} {ENVIRONMENT ENVDISPLAY_SET } {} {} {Include environment variable support} {} {} {} {Include environment} OC2000_CONFIG.OC2000_CONFIG_SCALING.INCLUDE_FILESYS.INCLUDE_ENVIRONMENT {} 0 bool
addvar INCLUDE_IEEE754$i $i {configos.h} u  "0" 6 {} {} {} {} {Include support of floating point exceptions conforming to IEEE754} {} {} {} {Include IEEE754} OC2000_CONFIG.OC2000_CONFIG_SCALING.INCLUDE_FILESYS.INCLUDE_IEEE754 {} 0 bool
addvar STREAM_MAX_CFG$i $i {configos.h} i  "100" 8 {} {} {} {} {The maximum number of streams may be open at one time} {} {} {} {Number of streams} OC2000_CONFIG.OC2000_CONFIG_SCALING.STREAM_MAX_CFG {} 0 int
addvar OPEN_MAX_CFG$i $i {configos.h} i  "200" 9 {} {} {} {$P >= 20} {The maximum number of files may be open at one time} {} {} {} {Number of open files} OC2000_CONFIG.OC2000_CONFIG_SCALING.OPEN_MAX_CFG {} 0 int
addvar VFS_CFG$i $i {configos.h} u  "1" 10 {} {} {} {} {} {} {} {} {Virtual filesystem configuration} OC2000_CONFIG.OC2000_CONFIG_SCALING.VFS_CFG {} 0 bool
addvar VFS_VNODES_MAX$i $i {configos.h} i  "100" 0 {} {} {} {} {} {} {} {} {Number of vnodes} OC2000_CONFIG.OC2000_CONFIG_SCALING.VFS_CFG.VFS_VNODES_MAX {} 0 int
addvar VFS_BUFFERS_MAX$i $i {configos.h} i  "100" 1 {} {} {} {} {} {} {} {} {Number of buffers} OC2000_CONFIG.OC2000_CONFIG_SCALING.VFS_CFG.VFS_BUFFERS_MAX {} 0 int
addvar VFS_MALLOC_MAX$i $i {configos.h} i  "819200" 2 {} {} {} {} {Maximum amount of memory allocated for buffers} {} {} {} {Amount of buffer space} OC2000_CONFIG.OC2000_CONFIG_SCALING.VFS_CFG.VFS_MALLOC_MAX {} 1 int
addvar MEMALLOC_CLEAR$i $i {configos.h} u  "0" 11 {} {} {} {} {Clear memory after memory deallocation} {} {} {} {Clear memory} OC2000_CONFIG.OC2000_CONFIG_SCALING.MEMALLOC_CLEAR {} 0 bool
addvar OC2000_CONFIG_A$i $i {configos.h} u  "1" 2 {} {} {} {} {} {guide/P_config_dial_libc.htm} {} {} {Standard C Libraries} OC2000_CONFIG.OC2000_CONFIG_A {} 0 bool
addvar INCLUDE_CTYPE$i $i {configos.h} u  "0" 0 {} {} {} {} {Include functions defined in ctype.h} {} {} {} {ctype.h} OC2000_CONFIG.OC2000_CONFIG_A.INCLUDE_CTYPE {} 0 bool
addvar INCLUDE_MATH$i $i {configos.h} u  "1" 1 {} {} {} {} {Include functions defined in math.h} {} {} {} {math.h} OC2000_CONFIG.OC2000_CONFIG_A.INCLUDE_MATH {} 0 bool
addvar INCLUDE_STDIO$i $i {configos.h} u  "0" 2 {} {} {} {} {Include functions defined in stdio.h} {} {} {} {stdio.h} OC2000_CONFIG.OC2000_CONFIG_A.INCLUDE_STDIO {} 0 bool
addvar INCLUDE_STDLIB$i $i {configos.h} u  "0" 3 {} {} {} {} {Include functions defined in stdlib.h} {} {} {} {stdlib.h} OC2000_CONFIG.OC2000_CONFIG_A.INCLUDE_STDLIB {} 0 bool
addvar INCLUDE_STRING$i $i {configos.h} u  "0" 4 {} {} {} {} {Include functions defined in string.h} {} {} {} {string.h} OC2000_CONFIG.OC2000_CONFIG_A.INCLUDE_STRING {} 0 bool
addvar INCLUDE_TIME$i $i {configos.h} u  "0" 5 {} {} {} {} {Include functions defined in time.h} {} {} {} {time.h} OC2000_CONFIG.OC2000_CONFIG_A.INCLUDE_TIME {} 0 bool
addvar INCLUDE_NETWORK$i $i {configos.h} u  "1" 3 {$INCLUDE_TIMERS==1 && $INCLUDE_POSIX_MQ==1} {INCLUDE_FTP_FS INCLUDE_NFS MOUNT_NFS_AUTO MOUNT_FTP INCLUDE_RDB INCLUDE_XCLIENT INCLUDE_XSERVER } {} {} {} {guide/P_config_dial_net.htm} {} {} {Network} OC2000_CONFIG.INCLUDE_NETWORK {} 0 bool
addvar NET_THREAD_PRIORITY$i $i {configos.h} i  "200" 0 {} {} {} {} {Priority of the inetd thread} {} {} {} {Inetd priority} OC2000_CONFIG.INCLUDE_NETWORK.NET_THREAD_PRIORITY {} 0 int
addvar INCLUDE_LOCAL_DOMAIN$i $i {configos.h} u  "1" 1 {} {} {} {} {Include loopback support} {} {} {} {Include loopback} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_LOCAL_DOMAIN {} 0 bool
addvar INCLUDE_PING$i $i {configos.h} u  "1" 2 {} {} {} {} {Include the ping function} {} {} {} {Include ping} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_PING {} 0 bool
addvar BOOTER_INHERITANCE$i $i {configos.h} u  "0" 3 {} {} {} {} {Inherit booter parameters} {} {} {} {Inherit booter parameters} OC2000_CONFIG.INCLUDE_NETWORK.BOOTER_INHERITANCE {} 0 bool
addvar INCLUDE_TELNET$i $i {configos.h} u  "0" 4 {} {} {} {} {} {} {} {} {Include telnet client} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_TELNET {} 0 bool
addvar INCLUDE_TELNETD$i $i {configos.h} u  "1" 5 {} {} {} {} {} {} {} {} {Include telnet server} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_TELNETD {} 0 bool
addvar TELNETD_PRIORITY$i $i {configos.h} i  "100" 0 {} {} {} {} {Telnet server priority} {} {} {} {Server priority} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_TELNETD.TELNETD_PRIORITY {} 0 int
addvar INCLUDE_RPC$i $i {configos.h} u  "1" 6 {} {INCLUDE_RDB } {} {} {Include RPC sever} {} {} {} {Include RPC} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_RPC {} 0 bool
addvar PORTMAPD_PRIORITY$i $i {configos.h} i  "127" 0 {} {} {} {$P >= 2 && $P <= 255} {} {} {} {} {portmapd priority} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_RPC.PORTMAPD_PRIORITY {} 0 int
addvar INCLUDE_NETTIME$i $i {configos.h} u  "1" 7 {} {} {} {} {Get time using Network Time Protocol and set system time} {} {} {} {Get time from Net} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_NETTIME {} 0 bool
addvar NETTIME_HOST_ADDRESS$i $i {configos.h} s  "host" 0 {} {} {} {} {Address of host to get time from} {} {} {} {Host address} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_NETTIME.NETTIME_HOST_ADDRESS {} 0 text
addvar NETTIME_ATTEMPT_COUNT$i $i {configos.h} i  "3" 1 {} {} {} {} {} {} {} {} {Number of attempts} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_NETTIME.NETTIME_ATTEMPT_COUNT {} 0 int
addvar NETTIME_TIMEOUT$i $i {configos.h} i  "3" 2 {} {} {} {} {} {} {} {} {Timeout in sec} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_NETTIME.NETTIME_TIMEOUT {} 0 int
addvar CONFIG_NETADDRESSES$i $i {configos.h} u  "0" 8 {} {} {} {} {} {guide/P_config_dial_net_adr.htm} {} {} {Network addresses} OC2000_CONFIG.INCLUDE_NETWORK.CONFIG_NETADDRESSES {} 0 bool
addvar INCLUDE_DEFAULT_GATEWAY$i $i {configos.h} u  "1" 0 {} {} {} {} {} {} {} {} {Default gateway} OC2000_CONFIG.INCLUDE_NETWORK.CONFIG_NETADDRESSES.INCLUDE_DEFAULT_GATEWAY {} 0 bool
addvar DEFAULT_GATEWAY_ADDRESS$i $i {configos.h} s  "192.168.0.1" 0 {} {} {} {[ipvalidate $P]} {IP address in dotted decimal notation} {} {} {} {Gateway address} OC2000_CONFIG.INCLUDE_NETWORK.CONFIG_NETADDRESSES.INCLUDE_DEFAULT_GATEWAY.DEFAULT_GATEWAY_ADDRESS {} 0 text
addvar INCLUDE_NETADDRESS$i $i {configos.h} u  "1" -1 {} {} {} {} {} {} {} {} {Network interfaces} OC2000_CONFIG.INCLUDE_NETWORK.CONFIG_NETADDRESSES.INCLUDE_NETADDRESS {} 0 bool
set ::current_val {{de0 192.168.0.7 255.255.255.0 0.0.0.0 0}         {tu0 192.168.0.7 255.255.255.0 0.0.0.0 1}         {ln0 192.168.0.7 255.255.255.0 0.0.0.0 2}         {bn0 192.168.202.1 255.255.255.0 0.0.0.0}         {sl0 192.168.1.7 255.255.255.0 192.168.1.1 3}         {ppp0 192.168.1.7 255.255.255.0 192.168.1.1}}
for { set i 0 } { $i < 6 } { incr i } {
addvar INCLUDE_NETADDRESS$i $i {configos.h} u  "[getdva INCLUDE_NETADDRESS $i]" -1 {} {} {} {} {} {} {} {} {Network interfaces} OC2000_CONFIG.INCLUDE_NETWORK.CONFIG_NETADDRESSES.INCLUDE_NETADDRESS {} 0 bool
addvar _IFNAME$i $i {configos.h} s  "de0" 0 {} {} {} {} {Name of the network interface} {} {} {} {Interface name} OC2000_CONFIG.INCLUDE_NETWORK.CONFIG_NETADDRESSES.INCLUDE_NETADDRESS._IFNAME { {de0}  {ln0}  {tu0}  {sm0}  {bn0} } 0 text
addvar _ADDRESS$i $i {configos.h} s  "192.168.0.7" 1 {} {} {} {[ipvalidate $P]} {IP address in dotted decimal notation} {} {} {} {IP address} OC2000_CONFIG.INCLUDE_NETWORK.CONFIG_NETADDRESSES.INCLUDE_NETADDRESS._ADDRESS {} 0 text
addvar _NETMASK$i $i {configos.h} s  "255.255.255.0" 2 {} {} {} {[ipvalidate $P]} {Net mask in dotted decimal notation} {} {} {} {Netmask} OC2000_CONFIG.INCLUDE_NETWORK.CONFIG_NETADDRESSES.INCLUDE_NETADDRESS._NETMASK {} 0 text
addvar _PTP$i $i {configos.h} s  "0.0.0.0" 3 {} {} {} {[ipvalidate $P]} {IP address of the remote side in dotted decimal notation (for SLIP and PPP)} {} {} {} {Partner address} OC2000_CONFIG.INCLUDE_NETWORK.CONFIG_NETADDRESSES.INCLUDE_NETADDRESS._PTP {} 0 text
}
set i {}
set ::current_val {}
addvar INCLUDE_HOST_TABLE_INIT$i $i {configos.h} u  "1" 9 {} {} {} {} {} {guide/P_config_dial_net_tab.htm} {} {} {Host table} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_HOST_TABLE_INIT {} 0 bool
addvar HOST_TABLE_NODES_COUNT$i $i {configos.h} i  "40" 0 {} {} {} {} {Maximum number of the table entries} {} {} {} {Number of entries} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_HOST_TABLE_INIT.HOST_TABLE_NODES_COUNT {} 0 int
addvar HOST_TABLE_ALIAS_COUNT$i $i {configos.h} i  "10" 1 {} {} {} {} {Maximum number of aliases per host} {} {} {} {Number of aliases} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_HOST_TABLE_INIT.HOST_TABLE_ALIAS_COUNT {} 0 int
addvar HOST_TABLE_ADDRESS_COUNT$i $i {configos.h} i  "1" 2 {} {} {} {} {Maximum number of IP addresses per host} {} {} {} {Number of IP addresses} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_HOST_TABLE_INIT.HOST_TABLE_ADDRESS_COUNT {} 0 int
addvar HOST_TABLE_NAME_LENGTH$i $i {configos.h} i  "40" 3 {} {} {} {} {Maximum length of host name} {} {} {} {Name length} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_HOST_TABLE_INIT.HOST_TABLE_NAME_LENGTH {} 0 int
addvar HOST_TABLE_ADDRESS_LENGTH$i $i {configos.h} i  "4" 4 {} {} {} {} {Maximum length of address} {} {} {} {Address length} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_HOST_TABLE_INIT.HOST_TABLE_ADDRESS_LENGTH {} 0 int
addvar INCLUDE_ADDHOSTS$i $i {configos.h} u  "1" -1 {} {} {} {} {} {} {} {} {Host list} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_HOST_TABLE_INIT.INCLUDE_ADDHOSTS { {1} } 0 bool
set ::current_val {}
for { set i 0 } { $i < 5 } { incr i } {
addvar INCLUDE_ADDHOSTS$i $i {configos.h} u  "[getdva INCLUDE_ADDHOSTS $i]" -1 {} {} {} {} {} {} {} {} {Host list} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_HOST_TABLE_INIT.INCLUDE_ADDHOSTS { {1} } 0 bool
addvar ADDHOSTS_NAME$i $i {configos.h} s  "host" 0 {} {} {} {} {} {} {} {} {Host name} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_HOST_TABLE_INIT.INCLUDE_ADDHOSTS.ADDHOSTS_NAME {} 0 text
addvar ADDHOSTS_ADDRESS$i $i {configos.h} s  "192.168.0.1" 1 {} {} {} {[ipvalidate $P]} {IP address in dotted decimal notation} {} {} {} {Host address} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_HOST_TABLE_INIT.INCLUDE_ADDHOSTS.ADDHOSTS_ADDRESS {} 0 text
}
set i {}
set ::current_val {}
addvar INCLUDE_NETGW$i $i {configos.h} u  "0" -1 {} {} {} {} {} {guide/P_config_dial_net_tab.htm} {} {} {Gateway table} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_NETGW {} 0 bool
set ::current_val {}
for { set i 0 } { $i < 6 } { incr i } {
addvar INCLUDE_NETGW$i $i {configos.h} u  "[getdva INCLUDE_NETGW $i]" -1 {} {} {} {} {} {guide/P_config_dial_net_tab.htm} {} {} {Gateway table} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_NETGW {} 0 bool
addvar GATEWAY_DST$i $i {configos.h} s  "192.168.0.1" 0 {} {} {} {[ipvalidate $P]} {IP address in dotted decimal notation} {} {} {} {Dst} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_NETGW.GATEWAY_DST {} 0 text
addvar GATEWAY_GW$i $i {configos.h} s  "192.168.0.1" 1 {} {} {} {[ipvalidate $P]} {IP address in dotted decimal notation} {} {} {} {Gw} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_NETGW.GATEWAY_GW {} 0 text
addvar GATEWAY_MASK$i $i {configos.h} s  "255.255.255.0" 2 {} {} {} {[ipvalidate $P]} {IP mask in dotted decimal notation} {} {} {} {Mask} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_NETGW.GATEWAY_MASK {} 0 text
}
set i {}
set ::current_val {}
addvar INCLUDE_SNETGW$i $i {configos.h} u  "0" -1 {} {} {} {} {} {guide/P_config_dial_net_tab.htm} {} {} {Static Routes table} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_SNETGW {} 0 bool
set ::current_val {}
for { set i 0 } { $i < 6 } { incr i } {
addvar INCLUDE_SNETGW$i $i {configos.h} u  "[getdva INCLUDE_SNETGW $i]" -1 {} {} {} {} {} {guide/P_config_dial_net_tab.htm} {} {} {Static Routes table} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_SNETGW {} 0 bool
addvar GATEWAY_SDST$i $i {configos.h} s  "192.168.0.1" 0 {} {} {} {[ipvalidate $P]} {IP address in dotted decimal notation} {} {} {} {IP Address} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_SNETGW.GATEWAY_SDST {} 0 text
addvar GATEWAY_SGW$i $i {configos.h} s  "00:00:00:00:00:00" 1 {} {} {} {} {Hardware address} {} {} {} {Mac Address} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_SNETGW.GATEWAY_SGW {} 0 text
}
set i {}
set ::current_val {}
addvar INCLUDE_SLIP$i $i {configos.h} u  "0" -1 {} {} {} {} {} {guide/P_config_dial_net_SLIP.htm} {} {} {SLIP protocol} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_SLIP {} 0 bool
set ::current_val {{/dev/aux0 296} {/dev/aux1 296} {/dev/aux2 296} {/dev/aux3 296}}
for { set i 0 } { $i < 4 } { incr i } {
addvar INCLUDE_SLIP$i $i {configos.h} u  "[getdva INCLUDE_SLIP $i]" -1 {} {} {} {} {} {guide/P_config_dial_net_SLIP.htm} {} {} {SLIP protocol} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_SLIP {} 0 bool
addvar SLIP_DEVNAME$i $i {configos.h} s  "/dev/aux1" 0 {} {} {} {} {} {} {} {} {Serial device name} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_SLIP.SLIP_DEVNAME {} 0 text
addvar SLIP_MTU$i $i {configos.h} i  "296" 1 {} {} {} {} {Maximum Transmission Unit} {} {} {} {MTU} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_SLIP.SLIP_MTU {} 0 int
}
set i {}
set ::current_val {}
addvar INCLUDE_PPP$i $i {configos.h} u  "0" -1 {} {} {} {} {} {guide/P_config_dial_net_PPP.htm} {} {} {PPP protocol} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_PPP {} 0 bool
set ::current_val {{/dev/aux0} {/dev/aux1} {/dev/aux2} {/dev/aux3}}
for { set i 0 } { $i < 4 } { incr i } {
addvar INCLUDE_PPP$i $i {configos.h} u  "[getdva INCLUDE_PPP $i]" -1 {} {} {} {} {} {guide/P_config_dial_net_PPP.htm} {} {} {PPP protocol} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_PPP {} 0 bool
addvar PPP_DEVNAME$i $i {configos.h} s  "/dev/aux1" 0 {} {} {} {} {} {} {} {} {Serial device name} OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_PPP.PPP_DEVNAME {} 0 text
}
set i {}
set ::current_val {}
addvar MBUF_CONFIG$i $i {configos.h} u  "1" 14 {} {} {} {} {} {guide/P_config_dial_buf.htm} {} {} {Network buffers} OC2000_CONFIG.INCLUDE_NETWORK.MBUF_CONFIG {} 0 bool
addvar NMB_INIT$i $i {configos.h} i  "16" 0 {} {} {} {} {Numbers of mbufs created at system initialization} {} {} {} {Init num of mbufs} OC2000_CONFIG.INCLUDE_NETWORK.MBUF_CONFIG.NMB_INIT {} 0 int
addvar NCL_INIT$i $i {configos.h} i  "1" 1 {} {} {} {} {Numbers of clusters created at system initialization} {} {} {} {Init num of clusters} OC2000_CONFIG.INCLUDE_NETWORK.MBUF_CONFIG.NCL_INIT {} 0 int
addvar NMB_ADD$i $i {configos.h} i  "16" 2 {} {} {} {} {Numbers of mbufs to be added in the case of lack of mbufs} {} {} {} {Num of mbufs add} OC2000_CONFIG.INCLUDE_NETWORK.MBUF_CONFIG.NMB_ADD {} 0 int
addvar NCL_ADD$i $i {configos.h} i  "1" 3 {} {} {} {} {Numbers of clusters to be added in the case of lack of clusters} {} {} {} {Num of clusters add} OC2000_CONFIG.INCLUDE_NETWORK.MBUF_CONFIG.NCL_ADD {} 0 int
addvar NMB_MAX$i $i {configos.h} i  "8192" 4 {} {} {} {} {Maximum number of mbufs} {} {} {} {Max num of mbufs} OC2000_CONFIG.INCLUDE_NETWORK.MBUF_CONFIG.NMB_MAX {} 0 int
addvar NCL_MAX$i $i {configos.h} i  "1024" 5 {} {} {} {} {Maximum number of clusters} {} {} {} {Max num of clusters} OC2000_CONFIG.INCLUDE_NETWORK.MBUF_CONFIG.NCL_MAX {} 0 int
addvar INCLUDE_EXTRA_FS$i $i {configos.h} u  "1" 4 {} {} {} {} {} {guide/P_config_dial_filesys.htm} {} {} {File systems} OC2000_CONFIG.INCLUDE_EXTRA_FS {} 0 bool
addvar MOUNT_VFAT$i $i {configos.h} u  "0" 0 {$INCLUDE_VFAT==1} {} {} {} {} {} {} {} {Mount VFAT} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_VFAT {} 0 bool
addvar VFAT_CHECK$i $i {configos.h} u  "1" 0 {} {} {} {} {} {} {} {} {Disk check options} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_VFAT.VFAT_CHECK {} 0 bool
addvar BFSC_OUT$i $i {configos.h} i  "BFSC_OUT_NORMAL" 0 {} {} {} {} {} {} {} {} {Output mode} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_VFAT.VFAT_CHECK.BFSC_OUT { {BFSC_OUT_SILENT}  {BFSC_OUT_NORMAL}  {BFSC_OUT_VERBOSE} } 0 int
addvar BFSC_MODE$i $i {configos.h} i  "BFSC_ALW_YES" 1 {} {} {} {} {} {} {} {} {Correction mode} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_VFAT.VFAT_CHECK.BFSC_MODE { {BFSC_ALW_YES}  {BFSC_ALW_NO}  {BFSC_INTERACTIVE} } 0 int
addvar BFSC_SAVE_BAD$i $i {configos.h} u  "0" 2 {} {} {} {} {Save bad chains to the 'BAD' directory} {} {} {} {Save bad chains} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_VFAT.VFAT_CHECK.BFSC_SAVE_BAD {} 0 bool
addvar BFSC_DO_CHECK_LEN$i $i {configos.h} u  "0" 3 {} {} {} {} {Check if stated file length is greater then actual} {} {} {} {Check file lengths} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_VFAT.VFAT_CHECK.BFSC_DO_CHECK_LEN {} 0 bool
addvar BFSC_DO_SYNC_FATS$i $i {configos.h} u  "1" 4 {} {} {} {} {Make FAT tables equal after check} {} {} {} {Synchronize FAT tables} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_VFAT.VFAT_CHECK.BFSC_DO_SYNC_FATS {} 0 bool
addvar VFAT_MNT_VOL$i $i {configos.h} u  "1" -1 {} {} {} {} {Mount the VFAT file system} {guide/P_config_dial_filesys_VFAT.htm} {} {} {Mount options} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_VFAT.VFAT_MNT_VOL {} 0 bool
set ::current_val {{/hda1 /dev/hda1 0 0 0 203 3 0777} {/hda2 /dev/hda2 0 0 0 203 3 0777} {/hdb1 /dev/hdb1 0 0 0 203 3 0777} {/hdb2 /dev/hdb2 0 0 0 203 3 0777}}
for { set i 0 } { $i < 4 } { incr i } {
addvar VFAT_MNT_VOL$i $i {configos.h} u  "[getdva VFAT_MNT_VOL $i]" -1 {} {} {} {} {Mount the VFAT file system} {guide/P_config_dial_filesys_VFAT.htm} {} {} {Mount options} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_VFAT.VFAT_MNT_VOL {} 0 bool
addvar VFAT_MOUNTPOINT$i $i {configos.h} s  "/vfat" 0 {} {} {^/[-._a-zA-Z0-9]+$} {} {Catalogue name where file system will be mounted} {} {} {} {Mount point} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_VFAT.VFAT_MNT_VOL.VFAT_MOUNTPOINT {} 0 text
addvar VFAT_BDNAME$i $i {configos.h} s  "/dev/hda" 1 {} {} {^/dev/[_a-zA-Z0-9]+$} {} {} {} {} {} {Block device name} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_VFAT.VFAT_MNT_VOL.VFAT_BDNAME {} 0 text
addvar VFAT_RDONLY$i $i {configos.h} i  "0" 2 {} {} {} {} {} {} {} {} {Read only} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_VFAT.VFAT_MNT_VOL.VFAT_RDONLY {} 0 bool
addvar VFAT_SYNCHRONOUS$i $i {configos.h} i  "0" 3 {} {} {} {} {} {} {} {} {FAT synchronous} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_VFAT.VFAT_MNT_VOL.VFAT_SYNCHRONOUS {} 0 bool
addvar VFAT_NOCHECK$i $i {configos.h} i  "0" 4 {} {} {} {} {} {} {} {} {No Check} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_VFAT.VFAT_MNT_VOL.VFAT_NOCHECK {} 0 bool
addvar VFAT_UID$i $i {configos.h} i  "203" 5 {} {} {} {} {} {} {} {} {UID} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_VFAT.VFAT_MNT_VOL.VFAT_UID {} 0 int
addvar VFAT_GID$i $i {configos.h} i  "3" 6 {} {} {} {} {} {} {} {} {GID} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_VFAT.VFAT_MNT_VOL.VFAT_GID {} 0 int
addvar VFAT_ACCESS$i $i {configos.h} i  "511" 7 {} {} {} {} {} {} {} {} {Permissions} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_VFAT.VFAT_MNT_VOL.VFAT_ACCESS {} 0 int
}
set i {}
set ::current_val {}
addvar MOUNT_CD9660$i $i {configos.h} u  "0" -1 {$INCLUDE_CD9660==1} {} {} {} {Mount the CD9660 file system} {guide/P_config_dial_filesys_VFAT.htm} {} {} {Mount CD-ISO9660} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_CD9660 {} 0 bool
set ::current_val {{/cd1 /dev/hda1} {/cd2 /dev/hda2} }
for { set i 0 } { $i < 2 } { incr i } {
addvar MOUNT_CD9660$i $i {configos.h} u  "[getdva MOUNT_CD9660 $i]" -1 {$INCLUDE_CD9660==1} {} {} {} {Mount the CD9660 file system} {guide/P_config_dial_filesys_VFAT.htm} {} {} {Mount CD-ISO9660} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_CD9660 {} 0 bool
addvar CD9660_MOUNTPOINT$i $i {configos.h} s  "" 0 {} {} {^/[-._a-zA-Z0-9]+$} {} {Catalogue name where file system will be mounted} {} {} {} {Mount point} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_CD9660.CD9660_MOUNTPOINT {} 0 text
addvar CD9660_BDNAME$i $i {configos.h} s  "" 1 {} {} {^/dev/[_a-zA-Z0-9]+$} {} {} {} {} {} {Block device name} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_CD9660.CD9660_BDNAME {} 0 text
}
set i {}
set ::current_val {}
addvar MOUNT_TAR$i $i {configos.h} u  "0" -1 {$INCLUDE_TAR_FS==1} {} {} {} {Mount the TAR file system} {guide/P_config_dial_filesys_TAR.htm} {} {} {Mount TAR} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_TAR {} 0 bool
set ::current_val {}
for { set i 0 } { $i < 4 } { incr i } {
addvar MOUNT_TAR$i $i {configos.h} u  "[getdva MOUNT_TAR $i]" -1 {$INCLUDE_TAR_FS==1} {} {} {} {Mount the TAR file system} {guide/P_config_dial_filesys_TAR.htm} {} {} {Mount TAR} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_TAR {} 0 bool
addvar TAR_MOUNTPOINT$i $i {configos.h} s  "/tar" 0 {} {} {^/[-._a-zA-Z0-9]+$} {} {Catalogue name where file system will be mounted} {} {} {} {Mount point} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_TAR.TAR_MOUNTPOINT {} 0 text
addvar TAR_MEMADDRESS$i $i {configos.h} x  "0x0" 1 {} {} {} {} {Address of memory area containing tared files} {} {} {} {Memory address} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_TAR.TAR_MEMADDRESS {} 0 int
addvar TAR_MEMSIZE$i $i {configos.h} x  "0x0" 2 {} {} {} {} {Size of memory area containing tared files} {} {} {} {Memory size} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_TAR.TAR_MEMSIZE {} 1 int
addvar TAR_CHECK$i $i {configos.h} x  "0" 3 {} {} {} {} {} {} {} {} {Check before mount} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_TAR.TAR_CHECK {} 0 bool
}
set i {}
set ::current_val {}
addvar INCLUDE_TARFILES$i $i {usermake.def} u  "0" -1 {$INCLUDE_TAR_FS==1} {} {} {} {Mount the TAR file system} {guide/P_config_dial_filesys_TAR.htm} {} {} {Mount TAR FILES} OC2000_CONFIG.INCLUDE_EXTRA_FS.INCLUDE_TARFILES {} 0 bool
set ::current_val {}
for { set i 0 } { $i < 4 } { incr i } {
addvar INCLUDE_TARFILES$i $i {usermake.def} u  "[getdva INCLUDE_TARFILES $i]" -1 {$INCLUDE_TAR_FS==1} {} {} {} {Mount the TAR file system} {guide/P_config_dial_filesys_TAR.htm} {} {} {Mount TAR FILES} OC2000_CONFIG.INCLUDE_EXTRA_FS.INCLUDE_TARFILES {} 0 bool
addvar TAR_PNAME$i $i {configos.h} s  "/tarfile" 0 {} {} {^/[-._a-zA-Z0-9]+$} {} {Catalogue name where file system will be mounted} {} {} {} {Mount point} OC2000_CONFIG.INCLUDE_EXTRA_FS.INCLUDE_TARFILES.TAR_PNAME {} 0 text
addvar TAR_FNAME$i $i {usermake.def} s  "" 1 {} {} {} {} {} {} {} {} {Tar Filename} OC2000_CONFIG.INCLUDE_EXTRA_FS.INCLUDE_TARFILES.TAR_FNAME {} 0 fn
addvar TAR_FCHECK$i $i {configos.h} x  "0" 2 {} {} {} {} {} {} {} {} {Check before mount} OC2000_CONFIG.INCLUDE_EXTRA_FS.INCLUDE_TARFILES.TAR_FCHECK {} 0 bool
}
set i {}
set ::current_val {}
addvar MOUNT_NFS_AUTO$i $i {configos.h} u  "1" 4 {$INCLUDE_NFS==1 && $INCLUDE_NETWORK==1} {} {} {} {Mount the NFS file system} {guide/P_config_dial_filesys_NFS.htm} {} {} {Mount NFS} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_NFS_AUTO {} 0 bool
addvar UID$i $i {configos.h} i  "203" 0 {} {} {} {} {User identifier} {} {} {} {User ID} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_NFS_AUTO.UID {} 0 int
addvar GID$i $i {configos.h} i  "3" 1 {} {} {} {} {Group identifier} {} {} {} {Group ID} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_NFS_AUTO.GID {} 0 int
addvar MOUNT_NFS$i $i {configos.h} u  "1" -1 {} {} {} {} {} {} {} {} {} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_NFS_AUTO.MOUNT_NFS {} 0 bool
set ::current_val {}
for { set i 0 } { $i < 4 } { incr i } {
addvar MOUNT_NFS$i $i {configos.h} u  "[getdva MOUNT_NFS $i]" -1 {} {} {} {} {} {} {} {} {} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_NFS_AUTO.MOUNT_NFS {} 0 bool
addvar NFS_HOST_ADDRESS$i $i {configos.h} s  "host" 0 {} {} {} {} {NFS server address (IP address in dotted decimal notation or name from the host table)} {} {} {} {Server address} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_NFS_AUTO.MOUNT_NFS.NFS_HOST_ADDRESS {} 0 text
addvar NFS_EXPORT$i $i {configos.h} s  "/tmp" 1 {} {} {} {} {Catalogue name on the NFS server} {} {} {} {Export name} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_NFS_AUTO.MOUNT_NFS.NFS_EXPORT {} 0 text
addvar NFS_MOUNTPOINT$i $i {configos.h} s  "/nfs" 2 {} {} {^/[-._a-zA-Z0-9]+$} {} {Catalogue name where file system will be mounted} {} {} {} {Mount point} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_NFS_AUTO.MOUNT_NFS.NFS_MOUNTPOINT {} 0 text
addvar NFS_RSIZE$i $i {configos.h} i  "0" 3 {} {} {} {} {NFS read buffer size} {} {} {} {NFS_RSIZE} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_NFS_AUTO.MOUNT_NFS.NFS_RSIZE {} 0 int
addvar NFS_WSIZE$i $i {configos.h} i  "0" 4 {} {} {} {} {NFS write buffer size} {} {} {} {NFS_WSIZE} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_NFS_AUTO.MOUNT_NFS.NFS_WSIZE {} 0 int
addvar NFS_FORCE$i $i {configos.h} i  "0" 5 {} {} {} {} {Wait for mounting forever} {} {} {} {NFS_FORCE} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_NFS_AUTO.MOUNT_NFS.NFS_FORCE {} 0 bool
}
set i {}
set ::current_val {}
addvar MOUNT_FTP$i $i {configos.h} u  "0" -1 {$INCLUDE_FTP_FS==1 && $INCLUDE_NETWORK==1} {} {} {} {Mount the FTP file system} {guide/P_config_dial_filesys_FTP.htm} {} {} {Mount FTP} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_FTP {} 0 bool
set ::current_val {}
for { set i 0 } { $i < 3 } { incr i } {
addvar MOUNT_FTP$i $i {configos.h} u  "[getdva MOUNT_FTP $i]" -1 {$INCLUDE_FTP_FS==1 && $INCLUDE_NETWORK==1} {} {} {} {Mount the FTP file system} {guide/P_config_dial_filesys_FTP.htm} {} {} {Mount FTP} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_FTP {} 0 bool
addvar FTP_MOUNTPOINT$i $i {configos.h} s  "/ftp" 0 {} {} {^/[-._a-zA-Z0-9]+$} {} {Catalogue name where file system will be mounted} {} {} {} {Mount point} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_FTP.FTP_MOUNTPOINT {} 0 text
addvar FTP_PARSTRING$i $i {configos.h} s  "user:password@hostname" 1 {} {} {} {} {Parameter string to log in FTP server} {} {} {} {Parameters} OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_FTP.FTP_PARSTRING {} 0 text
}
set i {}
set ::current_val {}
addvar INCLUDE_MULTIPROCESSING$i $i {usermake.def} u  "0" 5 {} {TRACE_MPMEM TRACE_MPSEM TRACE_MPMQ } {} {} {} {guide/P_config_dial_mp.htm} {} {} {Multiprocessing} OC2000_CONFIG.INCLUDE_MULTIPROCESSING {} 0 bool
addvar INCLUDE_MP_OBJS$i $i {usermake.def} u  "0" 0 {} {INCLUDE_CHANNELS TRACE_MPMEM TRACE_MPSEM TRACE_MPMQ } {} {} {Include shared memory object support} {} {} {} {Include shared mem objects} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_MP_OBJS {} 0 bool
addvar MASTER_CPU$i $i {configos.h} i  "0" 0 {} {} {} {} {Sequence number of the master CPU board} {} {} {} {Master CPU seq num} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_MP_OBJS.MASTER_CPU {} 0 int
addvar CPU_QUANTITY$i $i {configos.h} i  "2" 1 {} {} {} {} {Quantity of the CPU boards} {} {} {} {Num of CPU boards} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_MP_OBJS.CPU_QUANTITY {} 0 int
addvar MP_TCB_MAX$i $i {configos.h} i  "12" 2 {} {} {} {} {Maximum number of threads using shared memory objects} {} {} {} {Number of threads} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_MP_OBJS.MP_TCB_MAX {} 0 int
addvar MP_SEM_MAX$i $i {configos.h} i  "20" 3 {} {} {} {} {Maximum number of shared memory semaphores} {} {} {} {Number of semaphores} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_MP_OBJS.MP_SEM_MAX {} 0 int
addvar MP_MQP_MAX$i $i {configos.h} i  "10" 4 {} {} {} {} {Maximum number of shared memory message queues} {} {} {} {Number of msg queues} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_MP_OBJS.MP_MQP_MAX {} 0 int
addvar MP_DIC_MAX$i $i {configos.h} i  "40" 5 {} {} {} {} {Maximum number of dictionary entries} {} {} {} {Number of dic entries} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_MP_OBJS.MP_DIC_MAX {} 0 int
addvar INCLUDE_MPMEMPOOL_SELF$i $i {configos.h} u  "0" 6 {} {} {} {} {Create individual shared memory pool for this board} {} {} {} {Individual mem pool} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_MP_OBJS.INCLUDE_MPMEMPOOL_SELF {} 0 bool
addvar MP_LOCKTRIES_MAX$i $i {configos.h} i  "200" 7 {} {} {} {} {Maximum number of spin lock tries} {} {} {} {Number of lock tries} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_MP_OBJS.MP_LOCKTRIES_MAX {} 0 int
addvar INCLUDE_MPSHM$i $i {configos.h} u  "0" 8 {} {} {} {} {Include support of MIT shared memory extension} {} {} {} {MIT sh. memory ext.} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_MP_OBJS.INCLUDE_MPSHM {} 0 bool
addvar MP_NETWORK$i $i {configos.h} u  "0" 1 {} {} {} {} {Include Ethernet emulation on VME} {} {} {} {Ethernet emulation on VME} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.MP_NETWORK {} 0 bool
addvar MP_NETWORK_TYPE$i $i {configos.h} i  "MP_NETWORK_VITA" 0 {} {} {} {[setvar VITA_CONFIGURATION [expr [string compare $P MP_NETWORK_VITA]==0]]} {Implementation of Ethernet emulation on VME} {} {} {} {Implementation} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.MP_NETWORK.MP_NETWORK_TYPE { {MP_NETWORK_VITA}  {MP_NETWORK_BUSNET} } 0 combo
addvar VITA_CONFIGURATION$i $i {configos.h} u  "1" 1 {} {} {} {} {} {} {} {} {VITA} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.MP_NETWORK.VITA_CONFIGURATION {} 0 bool
addvar BN_MASTER_ADDR$i $i {configos.h} i  "0" 0 {} {} {} {} {} {} {} {} {Master addr} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.MP_NETWORK.VITA_CONFIGURATION.BN_MASTER_ADDR {} 0 int
addvar BN_PACKET_SIZE$i $i {configos.h} i  "2048" 1 {} {} {} {} {} {} {} {} {Packet size} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.MP_NETWORK.VITA_CONFIGURATION.BN_PACKET_SIZE {} 1 int
addvar BN_P_PROBE_STARTDELAY$i $i {configos.h} i  "1000000" 2 {} {} {} {} {} {} {} {} {Probe startdelay(mcs)} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.MP_NETWORK.VITA_CONFIGURATION.BN_P_PROBE_STARTDELAY {} 0 int
addvar BN_P_PROBE_MININTERVAL$i $i {configos.h} i  "1000000" 3 {} {} {} {} {} {} {} {} {Probe mininterval(mcs)} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.MP_NETWORK.VITA_CONFIGURATION.BN_P_PROBE_MININTERVAL {} 0 int
addvar BN_P_PROBE_MAXCOUNT$i $i {configos.h} i  "50" 4 {} {} {} {} {} {} {} {} {Probe maxcount} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.MP_NETWORK.VITA_CONFIGURATION.BN_P_PROBE_MAXCOUNT {} 0 int
addvar BN_P_PROBE_BACKOFFCOUNT$i $i {configos.h} i  "3" 5 {} {} {} {} {} {} {} {} {Probe backoff count} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.MP_NETWORK.VITA_CONFIGURATION.BN_P_PROBE_BACKOFFCOUNT {} 0 int
addvar BN_P_PROBE_MAXINTERVAL$i $i {configos.h} i  "20000000" 6 {} {} {} {} {} {} {} {} {Probe maxinterval(mcs)} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.MP_NETWORK.VITA_CONFIGURATION.BN_P_PROBE_MAXINTERVAL {} 0 int
addvar INCLUDE_CHANNELS$i $i {configos.h} u  "0" 2 {$INCLUDE_MP_OBJS==0} {} {} {} {Include channels support} {} {} {} {Include channels} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS {} 0 bool
addvar MPCHAN_THREAD_PRIORITY$i $i {configos.h} i  "101" 0 {} {} {} {$P >= 0 && $P <= 255} {MP-channels thread priority} {} {} {} {Thread priority} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.MPCHAN_THREAD_PRIORITY {} 0 int
addvar MPCHAN_MAX_NUMBER$i $i {configos.h} i  "5" 1 {} {} {} {$P >= 0 && $P <= 32} {Number of MP-channels} {} {} {} {Number of channels} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.MPCHAN_MAX_NUMBER {} 0 int
addvar INCLUDE_MPCHAN_VME$i $i {configos.h} u  "1" 2 {$INCLUDE_MPCHAN_RIO==0} {INCLUDE_MPCHAN_RIO } {} {} {Include VME channel support} {} {} {} {VME} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.INCLUDE_MPCHAN_VME {} 0 bool
addvar INCLUDE_MPCHAN_RIO$i $i {configos.h} u  "0" 3 {$INCLUDE_MPCHAN_VME==0} {INCLUDE_MPCHAN_VME } {} {} {Include RapidIO channel support} {} {} {} {RIO} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.INCLUDE_MPCHAN_RIO {} 0 bool
addvar INCLUDE_MPCHAN_NET$i $i {configos.h} u  "0" 4 {} {} {} {} {Include Net channel support} {} {} {} {NET} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.INCLUDE_MPCHAN_NET {} 0 bool
addvar MPCHAN_ETIMEDOUT_IGNORE$i $i {configos.h} u  "0" 5 {} {} {} {} {Ignore etimedout while sending multimodule message} {} {} {} {Ignore etimedout while sending multimodule message} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.MPCHAN_ETIMEDOUT_IGNORE {} 0 bool
addvar MPCHATHREAD_PERIODIC$i $i {configos.h} u  "0" 6 {} {} {} {} {} {} {} {} {mpchanThread periodic} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.MPCHATHREAD_PERIODIC {} 0 bool
addvar MPCHATHREAD_PERIOD$i $i {configos.h} i  "0" 0 {} {} {} {} {mpchanThread period} {} {} {} {Period (nsec)} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.MPCHATHREAD_PERIODIC.MPCHATHREAD_PERIOD {} 0 int
addvar INCLUDE_ARINC_QPORT$i $i {configos.h} u  "0" 7 {} {} {} {} {Include queuing port support} {} {} {} {Queuing ports} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.INCLUDE_ARINC_QPORT {} 0 bool
addvar QPORTLIST_MAX_CFG$i $i {configos.h} i  "5" 0 {} {} {} {} {Maximum number of queuing port list} {} {} {} {Number of queuing port list} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.INCLUDE_ARINC_QPORT.QPORTLIST_MAX_CFG {} 0 int
addvar QPORT_OPEN_MAX_CFG$i $i {configos.h} i  "32" 1 {} {} {} {$P >= 32 && $P <= 32} {Maximum number of } {} {} {} {Number of } OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.INCLUDE_ARINC_QPORT.QPORT_OPEN_MAX_CFG {} 0 int
addvar KERNEL_QPORT_CONFIG$i $i {configos.h} u  "1" -1 {} {} {} {} {Include queuing ports support for privileged system partition} {} {} {} {Queuing ports} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.INCLUDE_ARINC_QPORT.KERNEL_QPORT_CONFIG {} 0 bool
set ::current_val {}
for { set i 0 } { $i < 32 } { incr i } {
addvar KERNEL_QPORT_CONFIG$i $i {configos.h} i  "[getdva KERNEL_QPORT_CONFIG $i]" -1 {} {} {} {} {Include queuing ports support for privileged system partition} {} {} {} {Queuing ports} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.INCLUDE_ARINC_QPORT.KERNEL_QPORT_CONFIG {} 0 bool
addvar KERNEL_QPORT_NAME$i $i {configos.h} s  "" 0 {} {} {[-._a-zA-Z0-9]+$} {} {Port name} {} {} {} {Name} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.INCLUDE_ARINC_QPORT.KERNEL_QPORT_CONFIG.KERNEL_QPORT_NAME {} 0 text
addvar KERNEL_QPORT_DIRECT$i $i {configos.h} i  "SOURCE" 1 {} {} {} {} {Transfer direction} {} {} {} {Direction} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.INCLUDE_ARINC_QPORT.KERNEL_QPORT_CONFIG.KERNEL_QPORT_DIRECT { {SOURCE}  {DESTINATION} } 0 combo
addvar KERNEL_QPORT_BUFNUMBER$i $i {configos.h} i  "0" 2 {} {} {} {} {Maximum number of messages} {} {} {} {Buffer number} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.INCLUDE_ARINC_QPORT.KERNEL_QPORT_CONFIG.KERNEL_QPORT_BUFNUMBER {} 0 int
addvar KERNEL_QPORT_BUFSIZE$i $i {configos.h} i  "0" 3 {} {} {} {} {Maximum size of messages (bytes)} {} {} {} {Buffer size} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.INCLUDE_ARINC_QPORT.KERNEL_QPORT_CONFIG.KERNEL_QPORT_BUFSIZE {} 0 int
}
set i {}
set ::current_val {}
addvar INCLUDE_ARINC_SPORT$i $i {configos.h} u  "1" 8 {} {} {} {} {Include sampling port support} {} {} {} {Sampling ports} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.INCLUDE_ARINC_SPORT {} 0 bool
addvar SPORT_OPEN_MAX_CFG$i $i {configos.h} i  "32" 0 {} {} {} {$P >= 32 && $P <= 32} {Maximum number of } {} {} {} {Number of } OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.INCLUDE_ARINC_SPORT.SPORT_OPEN_MAX_CFG {} 0 int
addvar KERNEL_SPORT_CONFIG$i $i {configos.h} u  "1" -1 {} {} {} {} {Include sampling ports support for privileged system partition} {} {} {} {Sampling ports} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.INCLUDE_ARINC_SPORT.KERNEL_SPORT_CONFIG {} 0 bool
set ::current_val {}
for { set i 0 } { $i < 32 } { incr i } {
addvar KERNEL_SPORT_CONFIG$i $i {configos.h} i  "[getdva KERNEL_SPORT_CONFIG $i]" -1 {} {} {} {} {Include sampling ports support for privileged system partition} {} {} {} {Sampling ports} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.INCLUDE_ARINC_SPORT.KERNEL_SPORT_CONFIG {} 0 bool
addvar KERNEL_SPORT_NAME$i $i {configos.h} s  "" 0 {} {} {[-._a-zA-Z0-9]+$} {} {Port name} {} {} {} {Name} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.INCLUDE_ARINC_SPORT.KERNEL_SPORT_CONFIG.KERNEL_SPORT_NAME {} 0 text
addvar KERNEL_SPORT_DIRECT$i $i {configos.h} i  "SOURCE" 1 {} {} {} {} {Transfer direction} {} {} {} {Direction} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.INCLUDE_ARINC_SPORT.KERNEL_SPORT_CONFIG.KERNEL_SPORT_DIRECT { {SOURCE}  {DESTINATION} } 0 combo
addvar KERNEL_SPORT_BUFSIZE$i $i {configos.h} i  "0" 2 {} {} {} {} {Maximum size of messages (bytes)} {} {} {} {Buffer size} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.INCLUDE_ARINC_SPORT.KERNEL_SPORT_CONFIG.KERNEL_SPORT_BUFSIZE {} 0 int
addvar KERNEL_SPORT_REFRESH$i $i {configos.h} f  "0" 3 {} {} {} {} {Refresh period (seconds)} {} {} {} {Refresh period} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.INCLUDE_ARINC_SPORT.KERNEL_SPORT_CONFIG.KERNEL_SPORT_REFRESH {} 0 float
}
set i {}
set ::current_val {}
addvar CONNECTION_CONFIG$i $i {configos.h} u  "1" -1 {} {} {} {} {} {} {} {} {Connection} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.CONNECTION_CONFIG {} 0 bool
set ::current_val {}
for { set i 0 } { $i < 32 } { incr i } {
addvar CONNECTION_CONFIG$i $i {configos.h} i  "[getdva CONNECTION_CONFIG $i]" -1 {} {} {} {} {} {} {} {} {Connection} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.CONNECTION_CONFIG {} 0 bool
addvar CONNECTION_CHANNEL_ID$i $i {configos.h} i  "1" 0 {} {} {} {$P > 0 && $P <= 255} {} {} {} {} {Channel id} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.CONNECTION_CONFIG.CONNECTION_CHANNEL_ID {} 0 int
addvar CONNECTION_CHANNEL_NAME$i $i {configos.h} s  "channel1" 1 {} {} {[-._a-zA-Z0-9]+$} {} {Channel name} {} {} {} {Name} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.CONNECTION_CONFIG.CONNECTION_CHANNEL_NAME {} 0 text
addvar CONNECTION_PORT_NAME$i $i {configos.h} s  "port1" 2 {} {} {[-._a-zA-Z0-9]+$} {} {} {} {} {} {Port name} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.CONNECTION_CONFIG.CONNECTION_PORT_NAME {} 0 text
addvar CONNECTION_DIRECT$i $i {configos.h} i  "SOURCE" 3 {} {} {} {} {Transfer direction} {} {} {} {Direction} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.CONNECTION_CONFIG.CONNECTION_DIRECT { {SOURCE}  {DESTINATION} } 0 combo
addvar CONNECTION_CPU$i $i {configos.h} i  "0" 4 {} {} {} {$P >= 0 && $P <= 7} {} {} {} {} {Cpu partner} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.CONNECTION_CONFIG.CONNECTION_CPU {} 0 int
addvar CONNECTION_NLINK$i $i {configos.h} i  "1" 5 {} {} {} {$P > 0 && $P <= 255} {} {} {} {} {Logical link} OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.CONNECTION_CONFIG.CONNECTION_NLINK {} 0 int
}
set i {}
set ::current_val {}
addvar OC2000_MISC$i $i {configos.h} u  "1" 6 {} {} {} {} {} {guide/P_config_dial_misc.htm} {} {} {Miscellaneous} OC2000_CONFIG.OC2000_MISC {} 0 bool
addvar ENVIRONMENT$i $i {configos.h} s  "TZ=MSK-3MSS-4,M3.5.0,M10.5.0" 0 {$INCLUDE_ENVIRONMENT==1} {} {} {} {Assignment of values to environment variables (name=value separated by space(s))} {} {} {} {Environment variable} OC2000_CONFIG.OC2000_MISC.ENVIRONMENT {} 0 text
addvar OC2000_APPLICATION$i $i {configos.h} u  "1" 7 {} {} {} {} {} {guide/P_config_dial_applic.htm} {} {} {Application} OC2000_CONFIG.OC2000_APPLICATION {} 0 bool
addvar INCLUDE_LOGO$i $i {configos.h} u  "1" 0 {} {} {} {} {Display logo while booting} {} {} {} {Display logo} OC2000_CONFIG.OC2000_APPLICATION.INCLUDE_LOGO {} 0 bool
addvar INCLUDE_DEMO_THREAD$i $i {configos.h} u  "0" 1 {} {} {} {} {Include and start demo thread} {} {} {} {Start demo thread} OC2000_CONFIG.OC2000_APPLICATION.INCLUDE_DEMO_THREAD {} 0 bool
addvar INCLUDE_USER_THREAD$i $i {configos.h} u  "0" 2 {} {} {} {} {} {} {} {} {Start user thread} OC2000_CONFIG.OC2000_APPLICATION.INCLUDE_USER_THREAD {} 0 bool
addvar USER_THREAD_ENTRY$i $i {configos.h} d  "osUserThread" 0 {} {} {} {} {Name of the user thread function} {} {} {} {User thread entry} OC2000_CONFIG.OC2000_APPLICATION.INCLUDE_USER_THREAD.USER_THREAD_ENTRY {} 0 text
addvar USER_THREAD_PRIORITY$i $i {configos.h} i  "100" 1 {} {} {} {$P >= 0 && $P <= 255} {} {} {} {} {User thread priority} OC2000_CONFIG.OC2000_APPLICATION.INCLUDE_USER_THREAD.USER_THREAD_PRIORITY {} 0 int
addvar MAKEFILE_DEFINITIONS$i $i {usermake.def} u  "1" 3 {} {} {} {} {make file definitions} {} {} {} {MAKEFILE_DEFINITIONS} OC2000_CONFIG.OC2000_APPLICATION.MAKEFILE_DEFINITIONS {} 0 bool
addvar USER_INCLUDES$i $i {usermake.def} s  "" 0 {} {} {} {} {List of directories to search include files} {} {} {} {Include files} OC2000_CONFIG.OC2000_APPLICATION.MAKEFILE_DEFINITIONS.USER_INCLUDES {} 0 dn
addvar USER_SOURCES$i $i {usermake.def} s  "" 1 {} {} {} {} {Source file list} {} {} {} {Source files} OC2000_CONFIG.OC2000_APPLICATION.MAKEFILE_DEFINITIONS.USER_SOURCES {} 0 fn
addvar USER_OBJECTS$i $i {usermake.def} s  "" 2 {} {} {} {} {Object file list} {} {} {} {Object files} OC2000_CONFIG.OC2000_APPLICATION.MAKEFILE_DEFINITIONS.USER_OBJECTS {} 0 fn
addvar USER_LIBRARIES$i $i {usermake.def} s  "" 3 {} {} {} {} {Library list} {} {} {} {Libraries} OC2000_CONFIG.OC2000_APPLICATION.MAKEFILE_DEFINITIONS.USER_LIBRARIES {} 0 fn
addvar EXTERN_SYMBOLS$i $i {usermake.def} s  "" 4 {} {} {} {} {Extern symbol list} {} {} {} {Extern symbols} OC2000_CONFIG.OC2000_APPLICATION.MAKEFILE_DEFINITIONS.EXTERN_SYMBOLS {} 0 text
addvar ADDED_CFLAGS$i $i {usermake.def} s  "" 5 {} {} {} {} {Additional C compiler parameters} {} {} {} {CC options} OC2000_CONFIG.OC2000_APPLICATION.MAKEFILE_DEFINITIONS.ADDED_CFLAGS { {-g -w -G8 -O2 -DUSER_VARIABLE=225} } 0 parm
addvar LNK_OPTIONS$i $i {usermake.def} s  "" 6 {} {} {} {} {Additional linker parameters} {} {} {} {LD options} OC2000_CONFIG.OC2000_APPLICATION.MAKEFILE_DEFINITIONS.LNK_OPTIONS {} 0 text
addvar STARTPOINT$i $i {usermake.def} x  "0x80010000" 7 {} {} {} {} {} {} {} {} {Load address} OC2000_CONFIG.OC2000_APPLICATION.MAKEFILE_DEFINITIONS.STARTPOINT {} 0 int
addvar OC2000_CONFIG_D$i $i {configos.h} u  "1" 8 {} {} {} {} {} {guide/P_config_dial_debug.htm} {} {} {Debug} OC2000_CONFIG.OC2000_CONFIG_D {} 0 bool
addvar DEFAULT_INTMODE$i $i {configos.h} i  "INT_MODE_SUSPEND" 0 {} {} {} {} {Exceptions processing mode} {} {} {} {Exceptions processing} OC2000_CONFIG.OC2000_CONFIG_D.DEFAULT_INTMODE { {INT_MODE_SUSPEND}  {INT_MODE_POSIX}  {INT_MODE_REBOOT} } 0 combo
addvar INCLUDE_SYMTBL$i $i {configos.h} u  "1" 1 {} {INCLUDE_SHELL INCLUDE_XCLIENT } {} {} {Include symbol table} {} {} {} {Include symtable} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SYMTBL {} 0 bool
addvar INCLUDE_LOADER$i $i {configos.h} u  "1" 2 {} {INCLUDE_RDB } {} {} {Include dynamic loader} {} {} {} {Include loader} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_LOADER {} 0 bool
addvar INCLUDE_SHELL$i $i {configos.h} u  "1" 3 {$INCLUDE_SYMTBL==1} {INCLUDE_SHOW_MEMORY INCLUDE_SHOW_NETWORK } {} {} {Include shell thread} {} {} {} {Include shell} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SHELL {} 0 bool
addvar SHELL_PRIORITY$i $i {configos.h} i  "200" 0 {} {} {} {} {Priority of the shell thread} {} {} {} {Shell priority} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SHELL.SHELL_PRIORITY {} 0 int
addvar SHELL_STACK_SIZE$i $i {configos.h} i  "32768" 1 {} {} {} {} {Stack size of the shell thread} {} {} {} {Shell stack size} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SHELL.SHELL_STACK_SIZE {} 1 int
addvar SHELL_STARTUP_SCRIPT$i $i {configos.h} s  "" 2 {} {} {} {} {Shell startup script} {} {} {} {Shell script} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SHELL.SHELL_STARTUP_SCRIPT {} 0 text
addvar INCLUDE_DEBUG$i $i {configos.h} u  "1" 4 {$INCLUDE_POSIX_SEMAPHORES} {INCLUDE_RDB } {} {} {Include local debugger} {} {} {} {Include loc debugger} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_DEBUG {} 0 bool
addvar DEBUG_PRIORITY$i $i {configos.h} i  "236" 0 {} {} {} {$P >= 2 && $P <= 255} {Priority of the debug daemon thread} {} {} {} {debug-daemon priority} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_DEBUG.DEBUG_PRIORITY {} 0 int
addvar INCLUDE_RDB$i $i {configos.h} u  "1" 5 {$INCLUDE_DEBUG && $INCLUDE_NETWORK==1 && $INCLUDE_RPC==1 && $INCLUDE_LOADER==1} {} {} {} {Include remote debugger} {} {} {} {Include rem debugger} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_RDB {} 0 bool
addvar RDBTHREAD_PRIORITY$i $i {configos.h} i  "235" 0 {} {} {} {$P >= 2 && $P <= 255} {} {} {} {} {rdbThread priority} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_RDB.RDBTHREAD_PRIORITY {} 0 int
addvar RDBFUNCTION_PRIORITY$i $i {configos.h} i  "50" 1 {} {} {} {$P >= 2 && $P <= 255} {} {} {} {} {rdbFunction priority} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_RDB.RDBFUNCTION_PRIORITY {} 0 int
addvar RDBFUNCTION_STACKSIZE$i $i {configos.h} i  "32768" 2 {} {} {} {} {} {} {} {} {rdbFunction stack size} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_RDB.RDBFUNCTION_STACKSIZE {} 1 int
addvar INCLUDE_SHOW_OBJECTS$i $i {configos.h} u  "1" 6 {} {} {} {} {Include shell commands providing OS object information} {} {} {} {Object info commands} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SHOW_OBJECTS {} 0 bool
addvar INCLUDE_SHOW_MEMORY$i $i {configos.h} u  "1" 7 {$INCLUDE_SHELL==1} {} {} {} {Include shell commands providing memory information} {} {} {} {Memory info commands} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SHOW_MEMORY {} 0 bool
addvar INCLUDE_SHOW_NETWORK$i $i {configos.h} u  "1" 8 {$INCLUDE_SHELL==1} {} {} {} {Include shell commands providing network information} {} {} {} {Network info commands} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SHOW_NETWORK {} 0 bool
addvar MEMORY_CHECK$i $i {configos.h} u  "0" 9 {} {} {} {} {} {} {} {} {Check memory pool} OC2000_CONFIG.OC2000_CONFIG_D.MEMORY_CHECK {} 0 bool
addvar MEMORY_ERROR_PROCESSIG$i $i {configos.h} u  "1" 0 {} {} {} {} {} {} {} {} {Memory error processing} OC2000_CONFIG.OC2000_CONFIG_D.MEMORY_CHECK.MEMORY_ERROR_PROCESSIG {} 0 bool
addvar MEMORY_ERROR_SUSPEND$i $i {configos.h} u  "1" 0 {} {} {} {} {Suspend thread in the case of allocation/deallocation error} {} {} {} {Thread suspend} OC2000_CONFIG.OC2000_CONFIG_D.MEMORY_CHECK.MEMORY_ERROR_PROCESSIG.MEMORY_ERROR_SUSPEND {} 0 bool
addvar MEMORY_ERROR_LOG$i $i {configos.h} u  "0" 1 {$INCLUDE_SYSLOG==1} {} {} {} {Log memory allocation/deallocation error} {} {} {} {Error logging} OC2000_CONFIG.OC2000_CONFIG_D.MEMORY_CHECK.MEMORY_ERROR_PROCESSIG.MEMORY_ERROR_LOG {} 0 bool
addvar INCLUDE_SHOW_INIT$i $i {configos.h} u  "0" 10 {} {} {} {} {Output system initializing messages} {} {} {} {Output init msg} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SHOW_INIT {} 0 bool
addvar DEBUG_LIBRARY$i $i {usermake.def} u  "1" 11 {} {INCLUDE_TRACE } {} {} {} {} {} {} {Debug library} OC2000_CONFIG.OC2000_CONFIG_D.DEBUG_LIBRARY {} 0 bool
addvar INCLUDE_SYSLOG$i $i {configos.h} u  "1" 12 {} {MEMORY_ERROR_LOG SYSTEM_TO_SYSLOG } {} {} {} {} {} {} {System logger} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SYSLOG {} 0 bool
addvar SYSLOG_PRIORITY$i $i {configos.h} i  "240" 0 {} {} {} {} {Priority of the system logger thread} {} {} {} {Syslog-daemon priority} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SYSLOG.SYSLOG_PRIORITY {} 0 int
addvar SYSLOG_BUF_SIZE_CFG$i $i {configos.h} i  "128" 1 {} {} {} {} {Syslog buffer size} {} {} {} {Buffer size} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SYSLOG.SYSLOG_BUF_SIZE_CFG {} 0 int
addvar SYSLOG_BUF_COUNT_CFG$i $i {configos.h} i  "100" 2 {} {} {} {} {Number of syslog buffers} {} {} {} {Number of syslog buffers} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SYSLOG.SYSLOG_BUF_COUNT_CFG {} 0 int
addvar NOMESSAGE_HEADER$i $i {configos.h} u  "0" 3 {} {} {} {} {The logged message will not include a message header} {} {} {} {No message header} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SYSLOG.NOMESSAGE_HEADER {} 0 bool
addvar OUTPUT_MSG_TO$i $i {configos.h} i  "console" 4 {} {OUTPUT_MSG_CONSOLE MEM_FULL_POLICY NAME_OUTSTREAM ERASE_MEMORY NAME_FMEMORY MOVE_MEMORY MOVE_TYPE MOVE_MEMORY_SIZE NAME_PMEMORY SYSLOG_OPEN_FUNCTION SYSLOG_OUTPUT_FUNCTION SYSLOG_CLOSE_FUNCTION } {} {} {Stream where the message will be output} {} {} {} {Output message to} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SYSLOG.OUTPUT_MSG_TO { {console}  {file}  {persistent_memory}  {flash_memory}  {user_functions} } 0 combo
addvar OUTPUT_MSG_CONSOLE$i $i {configos.h} u  "0" 5 {[string compare $OUTPUT_MSG_TO console]!=0} {} {} {} {Write syslog messages to console, as well} {} {} {} {Output message to console, as well} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SYSLOG.OUTPUT_MSG_CONSOLE {} 0 bool
addvar MEM_FULL_POLICY$i $i {configos.h} u  "0" 6 {[string compare $OUTPUT_MSG_TO persistent_memory]==0} {} {} {} {Wrap to beginning of the area on overflow} {} {} {} {Loop on overflow} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SYSLOG.MEM_FULL_POLICY {} 0 bool
addvar NAME_OUTSTREAM$i $i {configos.h} s  "" 7 {[string compare $OUTPUT_MSG_TO file]==0} {} {} {} {} {} {} {} {File name} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SYSLOG.NAME_OUTSTREAM {} 0 text
addvar ERASE_MEMORY$i $i {configos.h} u  "0" 8 {[string compare $OUTPUT_MSG_TO flash_memory]==0} {} {} {} {Erase flash memory assigned for system log before using it} {} {} {} {Erase flash memory} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SYSLOG.ERASE_MEMORY {} 0 bool
addvar NAME_FMEMORY$i $i {configos.h} s  "area_1" 9 {[string compare $OUTPUT_MSG_TO flash_memory]==0} {} {} {} {Name of flash memory area tobe used for system log} {} {} {} {Name of flash memory area} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SYSLOG.NAME_FMEMORY {} 0 text
addvar MOVE_MEMORY$i $i {configos.h} s  "" 10 {[string compare $OUTPUT_MSG_TO flash_memory]==0 || [string compare $OUTPUT_MSG_TO persistent_memory]==0} {} {} {} {Move system log messages from memory (persistent or flash) to specified file at the system startup} {} {} {} {Move syslog data to} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SYSLOG.MOVE_MEMORY {} 0 text
addvar MOVE_TYPE$i $i {configos.h} i  "begin" 11 {[string compare $OUTPUT_MSG_TO flash_memory]==0 || [string compare $OUTPUT_MSG_TO persistent_memory]==0} {MOVE_MEMORY_SIZE } {} {} {Data can be written to the begining or the end of file or can be looped} {} {} {} {File write policy} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SYSLOG.MOVE_TYPE { {begin}  {end}  {loop} } 0 combo
addvar MOVE_MEMORY_SIZE$i $i {configos.h} x  "0x1000000" 12 {[string compare $MOVE_TYPE loop]==0 && [string compare $OUTPUT_MSG_TO flash_memory]==0 || [string compare $OUTPUT_MSG_TO persistent_memory]==0} {} {} {} {The size of the circuler buffer file to which data should be written} {} {} {} {Circuler buffer file size} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SYSLOG.MOVE_MEMORY_SIZE {} 1 int
addvar NAME_PMEMORY$i $i {configos.h} s  "SYSLOGMEM" 13 {[string compare $OUTPUT_MSG_TO persistent_memory]==0} {} {} {} {Name of persistent memory area to be used for system log} {} {} {} {Name of persistent memory area} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SYSLOG.NAME_PMEMORY {} 0 text
addvar SYSLOG_OPEN_FUNCTION$i $i {configos.h} d  "none" 14 {[string compare $OUTPUT_MSG_TO user_functions]==0} {} {} {} {Function to be called at file open} {} {} {} {Syslog user open function} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SYSLOG.SYSLOG_OPEN_FUNCTION {} 0 text
addvar SYSLOG_OUTPUT_FUNCTION$i $i {configos.h} d  "none" 15 {[string compare $OUTPUT_MSG_TO user_functions]==0} {} {} {} {The function to be called to output messages} {} {} {} {Syslog user output function} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SYSLOG.SYSLOG_OUTPUT_FUNCTION {} 0 text
addvar SYSLOG_CLOSE_FUNCTION$i $i {configos.h} d  "none" 16 {[string compare $OUTPUT_MSG_TO user_functions]==0} {} {} {} {The function to be called to close system log} {} {} {} {Syslog user close function} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SYSLOG.SYSLOG_CLOSE_FUNCTION {} 0 text
addvar INCLUDE_VALIDATION$i $i {usermake.def} u  "0" 13 {} {HM_CHECK_OBJECTS } {} {} {Enable self-test facilities} {} {} {} {Self-test facilities} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_VALIDATION {} 0 bool
addvar REPORT_ERRORS$i $i {configos.h} u  "1" 14 {} {} {} {} {} {} {} {} {Report exceptions} OC2000_CONFIG.OC2000_CONFIG_D.REPORT_ERRORS {} 0 bool
addvar INCLUDE_REPORT_DISASS$i $i {configos.h} u  "1" 0 {} {} {} {} {Report disassemble source code on exceptions} {} {} {} {Disassemble source code} OC2000_CONFIG.OC2000_CONFIG_D.REPORT_ERRORS.INCLUDE_REPORT_DISASS {} 0 bool
addvar INCLUDE_REPORT_BACKTR$i $i {configos.h} u  "1" 1 {} {} {} {} {Report stack backtrace on exceptions} {} {} {} {Stack backtrace} OC2000_CONFIG.OC2000_CONFIG_D.REPORT_ERRORS.INCLUDE_REPORT_BACKTR {} 0 bool
addvar INCLUDE_REPORT_THREADS$i $i {configos.h} u  "0" 2 {} {} {} {} {Report threads list on exceptions} {} {} {} {Threads list} OC2000_CONFIG.OC2000_CONFIG_D.REPORT_ERRORS.INCLUDE_REPORT_THREADS {} 0 bool
addvar INCLUDE_REPORT_FLREGS$i $i {configos.h} u  "0" 3 {} {} {} {} {Report floating-point registers on exceptions} {} {} {} {Floating-point registers} OC2000_CONFIG.OC2000_CONFIG_D.REPORT_ERRORS.INCLUDE_REPORT_FLREGS {} 0 bool
addvar HM_CHECK_OBJECTS$i $i {configos.h} i  "0" 4 {$INCLUDE_VALIDATION == 1} {} {} {} {Check OS objects on error} {} {} {} {Check OS objects} OC2000_CONFIG.OC2000_CONFIG_D.REPORT_ERRORS.HM_CHECK_OBJECTS {} 0 bool
addvar HM_REPORT_USER$i $i {configos.h} i  "0" 5 {} {HM_FUNTION_NAME } {} {} {Function to be called when an error occurs} {} {} {} {User function} OC2000_CONFIG.OC2000_CONFIG_D.REPORT_ERRORS.HM_REPORT_USER {} 0 bool
addvar HM_FUNTION_NAME$i $i {configos.h} d  "none" 6 {$HM_REPORT_USER==1} {} {} {} {Function to be called when an error occurs} {} {} {} {User function name} OC2000_CONFIG.OC2000_CONFIG_D.REPORT_ERRORS.HM_FUNTION_NAME {} 0 text
addvar SIZE_FPAREA$i $i {configos.h} i  "0" 7 {} {} {} {} {Size of frame to be printed} {} {} {} {Size of fp area} OC2000_CONFIG.OC2000_CONFIG_D.REPORT_ERRORS.SIZE_FPAREA {} 0 int
addvar INCLUDE_PRSMEM$i $i {configos.h} u  "0" 15 {} {INCLUDE_TRACE } {} {} {} {} {} {} {Persistent memory} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_PRSMEM {} 0 bool
addvar PRSMEM_ALLOC$i $i {configos.h} i  "PRSMEM_ALLOC_AUTO" 0 {} {PRSMEM_ADDRESS } {} {} {} {} {} {} {Memory allocation method} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_PRSMEM.PRSMEM_ALLOC { {PRSMEM_ALLOC_AUTO}  {PRSMEM_ALLOC_MP}  {PRSMEM_ALLOC_ADR} } 0 combo
addvar PRSMEM_ADDRESS$i $i {configos.h} i  "0" 1 {![string compare $PRSMEM_ALLOC PRSMEM_ALLOC_ADR]} {} {} {} {} {} {} {} {Persistent memory address} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_PRSMEM.PRSMEM_ADDRESS {} 0 int
addvar PRSMEM_PART_CONFIG$i $i {configos.h} u  "1" -1 {} {} {} {} {} {} {} {} {Persistent memory areas} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_PRSMEM.PRSMEM_PART_CONFIG {} 0 bool
set ::current_val {{TRACEBUF 409600}             {TRACELEGACY 1024}             {SYSLOGMEM 262144}          }
for { set i 0 } { $i < 8 } { incr i } {
addvar PRSMEM_PART_CONFIG$i $i {configos.h} i  "[getdva PRSMEM_PART_CONFIG $i]" -1 {} {} {} {} {} {} {} {} {Persistent memory areas} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_PRSMEM.PRSMEM_PART_CONFIG {} 0 bool
addvar PRSMEM_PART_NAME$i $i {configos.h} s  "" 0 {} {} {[-._a-zA-Z0-9]+$} {} {Partition name} {} {} {} {Name} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_PRSMEM.PRSMEM_PART_CONFIG.PRSMEM_PART_NAME {} 0 text
addvar PRSMEM_PART_SIZE$i $i {configos.h} i  "0" 1 {} {} {} {} {Partition size} {} {} {} {Size} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_PRSMEM.PRSMEM_PART_CONFIG.PRSMEM_PART_SIZE {} 1 int
}
set i {}
set ::current_val {}
addvar INCLUDE_FLASHMEM$i $i {configos.h} u  "0" 16 {} {} {} {} {} {} {} {} {Flash memory} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_FLASHMEM {} 0 bool
addvar FLASHMEM_PART_CONFIG$i $i {configos.h} u  "1" -1 {} {} {} {} {} {} {} {} {Flash memory areas} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_FLASHMEM.FLASHMEM_PART_CONFIG {} 0 bool
set ::current_val {{area_1 0 0}             {area_2 0 0}             {area_3 0 0}             {area_4 0 0}             {area_5 0 0}             {area_6 0 0}             {area_7 0 0}             {area_8 0 0}          }
for { set i 0 } { $i < 8 } { incr i } {
addvar FLASHMEM_PART_CONFIG$i $i {configos.h} i  "[getdva FLASHMEM_PART_CONFIG $i]" -1 {} {} {} {} {} {} {} {} {Flash memory areas} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_FLASHMEM.FLASHMEM_PART_CONFIG {} 0 bool
addvar FLASHMEM_PART_NAME$i $i {configos.h} s  "" 0 {} {} {[-._a-zA-Z0-9]+$} {} {Name of flash memory area} {} {} {} {Name} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_FLASHMEM.FLASHMEM_PART_CONFIG.FLASHMEM_PART_NAME {} 0 text
addvar FLASHMEM_PART_OFFSET$i $i {configos.h} x  "0x0" 1 {} {} {} {} {Offset of flash memory area relative to base address} {} {} {} {Offset} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_FLASHMEM.FLASHMEM_PART_CONFIG.FLASHMEM_PART_OFFSET {} 0 int
addvar FLASHMEM_PART_SIZE$i $i {configos.h} i  "0" 2 {} {} {} {} {Number of flash memory sectors in area} {} {} {} {Number of sectors} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_FLASHMEM.FLASHMEM_PART_CONFIG.FLASHMEM_PART_SIZE {} 0 int
}
set i {}
set ::current_val {}
addvar INCLUDE_SYSSHELL$i $i {configos.h} u  "1" 17 {} {} {} {} {} {} {} {} {Include sysShell-daemon} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SYSSHELL {} 0 bool
addvar SYSSHELL_PRIORITY$i $i {configos.h} i  "202" 0 {} {} {} {} {Priority of the sysShell thread} {} {} {} {sysShell-daemon priority} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SYSSHELL.SYSSHELL_PRIORITY {} 0 int
addvar SYSTEM_BUF_SIZE$i $i {configos.h} i  "128" 1 {} {} {} {} {Buffer size for commands} {} {} {} {Buffer size} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SYSSHELL.SYSTEM_BUF_SIZE {} 0 int
addvar SYSTEM_BUF_COUNT$i $i {configos.h} i  "10" 2 {} {} {} {} {Number of buffers for commands} {} {} {} {Number of buffers(sysShell)} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SYSSHELL.SYSTEM_BUF_COUNT {} 0 int
addvar SYSTEM_TO_SYSLOG$i $i {configos.h} i  "0" 3 {$INCLUDE_SYSLOG==1} {} {} {} {Redirect output of commands to syslog} {} {} {} {Redirect output to syslog} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SYSSHELL.SYSTEM_TO_SYSLOG {} 0 bool
addvar SYSTEM_PRINT$i $i {configos.h} i  "0" 4 {} {} {} {} {Print the command to be executed} {} {} {} {Print the command} OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SYSSHELL.SYSTEM_PRINT {} 0 bool
addvar INCLUDE_TRACE$i $i {usermake.def} u  "0" 9 {$DEBUG_LIBRARY==1 && $INCLUDE_PRSMEM==1 && $PRSMEM_PART_CONFIG0==1} {} {} {} {} {guide/P_config_dial_trace.htm} {} {} {Trace} OC2000_CONFIG.INCLUDE_TRACE {} 0 bool
addvar TRACE_NAME$i $i {configos.h} s  "trace" 0 {} {} {} {} {Name of the trace} {} {} {} {Trace name} OC2000_CONFIG.INCLUDE_TRACE.TRACE_NAME {} 0 text
addvar TRACE_CREATE$i $i {configos.h} u  "0" 1 {} {TRACE_AUTOSTART } {} {} {Create an active trace stream during the system initilization} {} {} {} {Trace stream initialization} OC2000_CONFIG.INCLUDE_TRACE.TRACE_CREATE {} 0 bool
addvar TRACE_AUTOSTART$i $i {configos.h} i  "0" 2 {$TRACE_CREATE==1} {} {} {} {Start tracing after initilisation} {} {} {} {Trace autostart} OC2000_CONFIG.INCLUDE_TRACE.TRACE_AUTOSTART {} 0 bool
addvar INCLUDE_TRACE_STREAM$i $i {configos.h} u  "1" 3 {} {} {} {} {} {} {} {} {} OC2000_CONFIG.INCLUDE_TRACE.INCLUDE_TRACE_STREAM {} 0 bool
addvar TRACE_STREAMFULLPOLICY$i $i {configos.h} i  "POSIX_TRACE_LOOP" 0 {} {} {} {} {The policy followed when the trace stream is full (stream-full-policy attribute)} {} {} {} {Stream-full-policy} OC2000_CONFIG.INCLUDE_TRACE.INCLUDE_TRACE_STREAM.TRACE_STREAMFULLPOLICY { {POSIX_TRACE_LOOP}  {POSIX_TRACE_UNTIL_FULL}  {POSIX_TRACE_FLUSH} } 0 combo
addvar TRACE_BLOCKS_SERVICE$i $i {configos.h} i  "3" 4 {} {} {} {$P >= 1 && $P <= 10} {Number of service blocks allocated for trace objects description} {} {} {} {Number of service blocks} OC2000_CONFIG.INCLUDE_TRACE.TRACE_BLOCKS_SERVICE {} 0 int
addvar INCLUDE_TRACE_LOG$i $i {configos.h} u  "0" 5 {$INCLUDE_VFAT==1 || $INCLUDE_NFS==1} {} {} {} {Associate a trace log with the trace stream} {} {} {} {Create trace log} OC2000_CONFIG.INCLUDE_TRACE.INCLUDE_TRACE_LOG {} 0 bool
addvar TRACE_LOGSIZE$i $i {configos.h} i  "409600" 0 {} {} {} {} {The maximum size in bytes of the trace log (log-max-size attribute)} {} {} {} {Log size} OC2000_CONFIG.INCLUDE_TRACE.INCLUDE_TRACE_LOG.TRACE_LOGSIZE {} 1 int
addvar TRACE_LOGFULLPOLICY$i $i {configos.h} i  "POSIX_TRACE_APPEND" 1 {} {} {} {} {The policy followed when the trace log is full (log-full-policy attribute)} {} {} {} {Log-full-policy} OC2000_CONFIG.INCLUDE_TRACE.INCLUDE_TRACE_LOG.TRACE_LOGFULLPOLICY { {POSIX_TRACE_APPEND}  {POSIX_TRACE_LOOP}  {POSIX_TRACE_UNTIL_FULL} } 0 combo
addvar TRACE_LOGPRIORITY$i $i {configos.h} i  "1" 2 {} {} {} {$P >= 0 && $P <= 255} {Priority of the thread writing trace stream to trace log} {} {} {} {Trace thread priority} OC2000_CONFIG.INCLUDE_TRACE.INCLUDE_TRACE_LOG.TRACE_LOGPRIORITY {} 0 int
addvar TRACE_LOGHIGHPRIORITY$i $i {configos.h} i  "188" 3 {} {} {} {$P >= $TRACE_LOGPRIORITY && $P <= 255} {Priority of the thread writing trace stream to trace log} {} {} {} {Trace thread high priority} OC2000_CONFIG.INCLUDE_TRACE.INCLUDE_TRACE_LOG.TRACE_LOGHIGHPRIORITY {} 0 int
addvar TRACE_LOG$i $i {configos.h} s  "/nfs/trace" 6 {$INCLUDE_VFAT==1 || $INCLUDE_NFS==1} {} {[-_a-zA-Z0-9/]+$} {} {Name of the trace log file} {} {} {} {Trace log name} OC2000_CONFIG.INCLUDE_TRACE.TRACE_LOG {} 0 text
addvar TRACE_LOGROTATE$i $i {configos.h} i  "0" 7 {} {} {} {} {Number of trace files (0-unlimited)} {} {} {} {Number of trace log files} OC2000_CONFIG.INCLUDE_TRACE.TRACE_LOGROTATE {} 0 int
addvar TRACE_MAXDATASIZE$i $i {configos.h} i  "100" 8 {} {} {} {$P >= 50 && $P <= 2000} {The maximum size for the user data (max-data-size attribute)} {} {} {} {User data size} OC2000_CONFIG.INCLUDE_TRACE.TRACE_MAXDATASIZE {} 0 int
addvar CONFIG_TRACE_EXCEPTION$i $i {configos.h} u  "1" 9 {} {} {} {} {Advanced exception tracing} {} {} {} {Exception tracing} OC2000_CONFIG.INCLUDE_TRACE.CONFIG_TRACE_EXCEPTION {} 0 bool
addvar TRACE_EXCEPTION_FP$i $i {configos.h} i  "0" 0 {} {} {} {} {Log contents of floating point register at exception time} {} {} {} {Floating point register} OC2000_CONFIG.INCLUDE_TRACE.CONFIG_TRACE_EXCEPTION.TRACE_EXCEPTION_FP {} 0 bool
addvar TRACE_EXCEPTION_CALLSTACK$i $i {configos.h} i  "0" 1 {} {} {} {} {} {} {} {} {Call stack} OC2000_CONFIG.INCLUDE_TRACE.CONFIG_TRACE_EXCEPTION.TRACE_EXCEPTION_CALLSTACK {} 0 bool
addvar TRACE_EXCEPTIONPOLICY$i $i {configos.h} i  "TRACE_EXCEPTION_NONE" 2 {} {} {} {} {Exception processing} {} {} {} {Exception policy} OC2000_CONFIG.INCLUDE_TRACE.CONFIG_TRACE_EXCEPTION.TRACE_EXCEPTIONPOLICY { {TRACE_EXCEPTION_NONE}  {TRACE_EXCEPTION_STOP}  {TRACE_EXCEPTION_SHUTDOWN}  {TRACE_EXCEPTION_REBOOT} } 0 combo
addvar TRACE_CHANNEL_CONFIG$i $i {configos.h} u  "0" -1 {} {} {} {} {Include channel tracing} {} {} {} {Channel tracing} OC2000_CONFIG.INCLUDE_TRACE.TRACE_CHANNEL_CONFIG {} 0 bool
set ::current_val {}
for { set i 0 } { $i < 8 } { incr i } {
addvar TRACE_CHANNEL_CONFIG$i $i {configos.h} i  "[getdva TRACE_CHANNEL_CONFIG $i]" -1 {} {} {} {} {Include channel tracing} {} {} {} {Channel tracing} OC2000_CONFIG.INCLUDE_TRACE.TRACE_CHANNEL_CONFIG {} 0 bool
addvar TRACE_CHANNEL_ID$i $i {configos.h} i  "0" 0 {} {} {} {} {Channel identificator} {} {} {} {Id} OC2000_CONFIG.INCLUDE_TRACE.TRACE_CHANNEL_CONFIG.TRACE_CHANNEL_ID {} 0 int
addvar TRACE_CHANMSG_SIZE$i $i {configos.h} i  "0" 1 {} {} {} {$P <= $TRACE_MAXDATASIZE} {Count of bytes which should be written} {} {} {} {Size} OC2000_CONFIG.INCLUDE_TRACE.TRACE_CHANNEL_CONFIG.TRACE_CHANMSG_SIZE {} 0 int
addvar TRACE_CHANMSG_HDR$i $i {configos.h} i  "0" 2 {} {} {} {} {Write header of message} {} {} {} {Header} OC2000_CONFIG.INCLUDE_TRACE.TRACE_CHANNEL_CONFIG.TRACE_CHANMSG_HDR {} 0 bool
}
set i {}
set ::current_val {}
addvar TRACE_APPEND1$i $i {configos.h} u  "1" 11 {} {} {} {} {} {} {} {} {} OC2000_CONFIG.INCLUDE_TRACE.TRACE_APPEND1 {} 0 bool
addvar TRACE_BUFFER_SAVE$i $i {configos.h} i  "0" 0 {} {} {} {} {Write to the log last portion of trace stream if it was not written before} {} {} {} {Save trace of previous session} OC2000_CONFIG.INCLUDE_TRACE.TRACE_APPEND1.TRACE_BUFFER_SAVE {} 0 bool
addvar TRACE_LOAD_CONFIG$i $i {configos.h} u  "0" 12 {$INCLUDE_VFAT==1 || $INCLUDE_NFS==1} {} {} {} {Include configuration loading} {} {} {} {Configuration load} OC2000_CONFIG.INCLUDE_TRACE.TRACE_LOAD_CONFIG {} 0 bool
addvar TRACE_CONFIG_FILE$i $i {configos.h} s  "/nfs/configos.h" 0 {} {} {[.-_a-zA-Z0-9/]+$} {} {} {} {} {} {Configuration file name} OC2000_CONFIG.INCLUDE_TRACE.TRACE_LOAD_CONFIG.TRACE_CONFIG_FILE {} 0 text
addvar TRACE_CONFIG_MASK_REGISTRATION$i $i {configos.h} s  "/nfs/trace.mask" 1 {} {} {[.-_a-zA-Z0-9/]+$} {} {} {} {} {} {Configuration mask events file name} OC2000_CONFIG.INCLUDE_TRACE.TRACE_LOAD_CONFIG.TRACE_CONFIG_MASK_REGISTRATION {} 0 text
addvar TRACE_EVENT$i $i {configos.h} u  "0" 13 {} {} {} {} {} {guide/P_config_dial_trace_filters.htm} {} {} {Event filter} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT {} 0 bool
addvar TRACE_GLB_INTERRUPT$i $i {configos.h} u  "0" 0 {} {} {} {} {} {guide/P_config_dial_trace_filters_int.htm} {} {} {Interrupts} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_INTERRUPT {} 0 bool
addvar TRACE_INTERRUPT_ENTRY$i $i {configos.h} u  "1" 0 {} {} {} {} {} {} {} {} {Interrupt entry} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_INTERRUPT.TRACE_INTERRUPT_ENTRY {} 0 bool
addvar TRACE_INTERRUPT_EXIT$i $i {configos.h} u  "1" 1 {} {} {} {} {} {} {} {} {Interrupt exit} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_INTERRUPT.TRACE_INTERRUPT_EXIT {} 0 bool
addvar TRACE_GLB_EXCEPTION$i $i {configos.h} u  "0" 1 {} {} {} {} {} {guide/P_config_dial_trace_filters_except.htm} {} {} {Exceptions} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_EXCEPTION {} 0 bool
addvar TRACE_GLB_THREADSWITCH$i $i {configos.h} u  "0" 2 {} {} {} {} {} {guide/P_config_dial_trace_filters_switch.htm} {} {} {Thread switch} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_THREADSWITCH {} 0 bool
addvar TRACE_THREADSWITCH$i $i {configos.h} u  "1" 0 {} {} {} {} {} {} {} {} {Thread switch} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_THREADSWITCH.TRACE_THREADSWITCH {} 0 bool
addvar TRACE_THREADIDLE$i $i {configos.h} u  "1" 1 {} {} {} {} {} {} {} {} {System idle} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_THREADSWITCH.TRACE_THREADIDLE {} 0 bool
addvar TRACE_GLB_THREADSTATUS$i $i {configos.h} u  "0" 3 {} {} {} {} {} {guide/P_config_dial_trace_filters_status.htm} {} {} {Thread status change} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_THREADSTATUS {} 0 bool
addvar TRACE_THREADCREATE$i $i {configos.h} u  "1" 0 {} {} {} {} {} {} {} {} {Thread create} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_THREADSTATUS.TRACE_THREADCREATE {} 0 bool
addvar TRACE_THREADEXIT$i $i {configos.h} u  "1" 1 {} {} {} {} {} {} {} {} {Thread exit} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_THREADSTATUS.TRACE_THREADEXIT {} 0 bool
addvar TRACE_THREADACTIVATE$i $i {configos.h} u  "1" 2 {} {} {} {} {} {} {} {} {Thread is ready} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_THREADSTATUS.TRACE_THREADACTIVATE {} 0 bool
addvar TRACE_THREADBLOCK$i $i {configos.h} u  "1" 3 {} {} {} {} {} {} {} {} {Thread is not ready} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_THREADSTATUS.TRACE_THREADBLOCK {} 0 bool
addvar TRACE_THREADSCHEDYIELD$i $i {configos.h} u  "1" 4 {} {} {} {} {} {} {} {} {sched_yield()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_THREADSTATUS.TRACE_THREADSCHEDYIELD {} 0 bool
addvar TRACE_THREADTIMEQUANTUM$i $i {configos.h} u  "1" 5 {} {} {} {} {} {} {} {} {Time quantum expired} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_THREADSTATUS.TRACE_THREADTIMEQUANTUM {} 0 bool
addvar TRACE_THREADSUSPEND$i $i {configos.h} u  "1" 6 {} {} {} {} {} {} {} {} {Thread suspend} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_THREADSTATUS.TRACE_THREADSUSPEND {} 0 bool
addvar TRACE_THREADRESUME$i $i {configos.h} u  "1" 7 {} {} {} {} {} {} {} {} {Thread resume} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_THREADSTATUS.TRACE_THREADRESUME {} 0 bool
addvar TRACE_GLB_SIGNAL$i $i {configos.h} u  "0" 4 {} {} {} {} {} {guide/P_config_dial_trace_filters_siggen.htm} {} {} {Signal generation and delivery} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_SIGNAL {} 0 bool
addvar TRACE_SIGGENERATION$i $i {configos.h} u  "1" 0 {} {} {} {} {} {} {} {} {Signal generation} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_SIGNAL.TRACE_SIGGENERATION {} 0 bool
addvar TRACE_SIGDELIVERY$i $i {configos.h} u  "1" 1 {} {} {} {} {} {} {} {} {Signal delivery} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_SIGNAL.TRACE_SIGDELIVERY {} 0 bool
addvar TRACE_SIGCATCHFUNC$i $i {configos.h} u  "1" 2 {} {} {} {} {} {} {} {} {Signal-catching function call} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_SIGNAL.TRACE_SIGCATCHFUNC {} 0 bool
addvar TRACE_SIGCATCHFUNCRET$i $i {configos.h} u  "1" 3 {} {} {} {} {} {} {} {} {Exit from signal-catching function} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_SIGNAL.TRACE_SIGCATCHFUNCRET {} 0 bool
addvar TRACE_SIGTHREADCANCEL$i $i {configos.h} u  "1" 4 {} {} {} {} {} {} {} {} {Thread cancellation request} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_SIGNAL.TRACE_SIGTHREADCANCEL {} 0 bool
addvar TRACE_GLB_SIGWAIT$i $i {configos.h} u  "0" 5 {} {} {} {} {} {guide/P_config_dial_trace_filters_sigwait.htm} {} {} {Wait for a signal} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_SIGWAIT {} 0 bool
addvar TRACE_SIGPAUSE$i $i {configos.h} u  "1" 0 {} {} {} {} {} {} {} {} {pause()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_SIGWAIT.TRACE_SIGPAUSE {} 0 bool
addvar TRACE_SIGSUSPEND$i $i {configos.h} u  "1" 1 {} {} {} {} {} {} {} {} {sigsuspend()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_SIGWAIT.TRACE_SIGSUSPEND {} 0 bool
addvar TRACE_SIGTIMEDWAIT$i $i {configos.h} u  "1" 2 {} {} {} {} {} {} {} {} {sigtimedwait()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_SIGWAIT.TRACE_SIGTIMEDWAIT {} 0 bool
addvar TRACE_SIGWAIT$i $i {configos.h} u  "1" 3 {} {} {} {} {} {} {} {} {sigwait()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_SIGWAIT.TRACE_SIGWAIT {} 0 bool
addvar TRACE_SIGWAITINFO$i $i {configos.h} u  "1" 4 {} {} {} {} {} {} {} {} {sigwaitinfo()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_SIGWAIT.TRACE_SIGWAITINFO {} 0 bool
addvar TRACE_GLB_MEMORY$i $i {configos.h} u  "0" 6 {} {} {} {} {} {guide/P_config_dial_trace_filters_mem.htm} {} {} {Memory allocation} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_MEMORY {} 0 bool
addvar TRACE_MEMPOOLCREATE$i $i {configos.h} u  "1" 0 {} {} {} {} {} {} {} {} {memPoolCreate()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_MEMORY.TRACE_MEMPOOLCREATE {} 0 bool
addvar TRACE_MEMPOOLADD$i $i {configos.h} u  "1" 1 {} {} {} {} {} {} {} {} {memPoolAdd()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_MEMORY.TRACE_MEMPOOLADD {} 0 bool
addvar TRACE_MEMPOOLDELETE$i $i {configos.h} u  "1" 2 {} {} {} {} {} {} {} {} {memPoolDelete()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_MEMORY.TRACE_MEMPOOLDELETE {} 0 bool
addvar TRACE_MEMPOOLALLOC$i $i {configos.h} u  "1" 3 {} {} {} {} {} {} {} {} {Mem alloc} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_MEMORY.TRACE_MEMPOOLALLOC {} 0 bool
addvar TRACE_MEMPOOLFREE$i $i {configos.h} u  "1" 4 {} {} {} {} {} {} {} {} {Mem free} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_MEMORY.TRACE_MEMPOOLFREE {} 0 bool
addvar TRACE_GLB_THREAD$i $i {configos.h} u  "0" 7 {} {} {} {} {} {guide/P_config_dial_trace_filters_thread.htm} {} {} {Thread control functions} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_THREAD {} 0 bool
addvar TRACE_THREADCANCEL$i $i {configos.h} u  "1" 0 {} {} {} {} {} {} {} {} {pthread_cancel()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_THREAD.TRACE_THREADCANCEL {} 0 bool
addvar TRACE_THREADJOIN$i $i {configos.h} u  "1" 1 {} {} {} {} {} {} {} {} {pthread_join()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_THREAD.TRACE_THREADJOIN {} 0 bool
addvar TRACE_THREADSIGMASK$i $i {configos.h} u  "1" 2 {} {} {} {} {} {} {} {} {pthread_sigmask()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_THREAD.TRACE_THREADSIGMASK {} 0 bool
addvar TRACE_THREADSETSCHEDPARAM$i $i {configos.h} u  "1" 3 {} {} {} {} {} {} {} {} {pthread_setschedparam()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_THREAD.TRACE_THREADSETSCHEDPARAM {} 0 bool
addvar TRACE_THREADSETCANCELSTATE$i $i {configos.h} u  "1" 4 {} {} {} {} {} {} {} {} {pthread_setcancelstate()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_THREAD.TRACE_THREADSETCANCELSTATE {} 0 bool
addvar TRACE_THREADSETCANCELTYPE$i $i {configos.h} u  "1" 5 {} {} {} {} {} {} {} {} {pthread_setcanceltype()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_THREAD.TRACE_THREADSETCANCELTYPE {} 0 bool
addvar TRACE_THREADDETACH$i $i {configos.h} u  "1" 6 {} {} {} {} {} {} {} {} {pthread_detach()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_THREAD.TRACE_THREADDETACH {} 0 bool
addvar TRACE_GLB_RETURN$i $i {configos.h} u  "0" 8 {} {} {} {} {} {guide/P_config_dial_trace_filters_return.htm} {} {} {Return} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_RETURN {} 0 bool
addvar TRACE_GLB_SEMAPHORE$i $i {configos.h} u  "0" 9 {} {} {} {} {} {guide/P_config_dial_trace_filters_sem.htm} {} {} {Semaphore management functions} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_SEMAPHORE {} 0 bool
addvar TRACE_SEMGETVALUE$i $i {configos.h} u  "1" 0 {} {} {} {} {} {} {} {} {sem_getvalue()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_SEMAPHORE.TRACE_SEMGETVALUE {} 0 bool
addvar TRACE_SEMOPEN$i $i {configos.h} u  "1" 1 {} {} {} {} {} {} {} {} {sem_open()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_SEMAPHORE.TRACE_SEMOPEN {} 0 bool
addvar TRACE_SEMCLOSE$i $i {configos.h} u  "1" 2 {} {} {} {} {} {} {} {} {sem_close()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_SEMAPHORE.TRACE_SEMCLOSE {} 0 bool
addvar TRACE_SEMUNLINK$i $i {configos.h} u  "1" 3 {} {} {} {} {} {} {} {} {sem_unlink()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_SEMAPHORE.TRACE_SEMUNLINK {} 0 bool
addvar TRACE_SEMINIT$i $i {configos.h} u  "1" 4 {} {} {} {} {} {} {} {} {sem_init()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_SEMAPHORE.TRACE_SEMINIT {} 0 bool
addvar TRACE_SEMDESTROY$i $i {configos.h} u  "1" 5 {} {} {} {} {} {} {} {} {sem_destroy()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_SEMAPHORE.TRACE_SEMDESTROY {} 0 bool
addvar TRACE_SEMPOST$i $i {configos.h} u  "1" 6 {} {} {} {} {} {} {} {} {sem_post()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_SEMAPHORE.TRACE_SEMPOST {} 0 bool
addvar TRACE_DELAYEDSEMPOST$i $i {configos.h} u  "1" 7 {} {} {} {} {} {} {} {} {Delayed post} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_SEMAPHORE.TRACE_DELAYEDSEMPOST {} 0 bool
addvar TRACE_SEMWAIT$i $i {configos.h} u  "1" 8 {} {} {} {} {} {} {} {} {sem_wait()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_SEMAPHORE.TRACE_SEMWAIT {} 0 bool
addvar TRACE_SEMTIMEDWAIT$i $i {configos.h} u  "1" 9 {} {} {} {} {} {} {} {} {sem_timedwait()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_SEMAPHORE.TRACE_SEMTIMEDWAIT {} 0 bool
addvar TRACE_SEMTRYWAIT$i $i {configos.h} u  "1" 10 {} {} {} {} {} {} {} {} {sem_trywait()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_SEMAPHORE.TRACE_SEMTRYWAIT {} 0 bool
addvar TRACE_GLB_MUTEX$i $i {configos.h} u  "0" 10 {} {} {} {} {} {guide/P_config_dial_trace_filters_mutex.htm} {} {} {Mutex management functions} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_MUTEX {} 0 bool
addvar TRACE_MUTEXDESTROY$i $i {configos.h} u  "1" 0 {} {} {} {} {} {} {} {} {pthread_mutex_destroy()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_MUTEX.TRACE_MUTEXDESTROY {} 0 bool
addvar TRACE_MUTEXINIT$i $i {configos.h} u  "1" 1 {} {} {} {} {} {} {} {} {pthread_mutex_init()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_MUTEX.TRACE_MUTEXINIT {} 0 bool
addvar TRACE_MUTEXDSETPRIOCEILING$i $i {configos.h} u  "1" 2 {} {} {} {} {} {} {} {} {pthread_mutex_setprioceiling()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_MUTEX.TRACE_MUTEXDSETPRIOCEILING {} 0 bool
addvar TRACE_MUTEXGETPRIOCEILING$i $i {configos.h} u  "1" 3 {} {} {} {} {} {} {} {} {pthread_mutex_getprioceiling()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_MUTEX.TRACE_MUTEXGETPRIOCEILING {} 0 bool
addvar TRACE_MUTEXLOCK$i $i {configos.h} u  "1" 4 {} {} {} {} {} {} {} {} {pthread_mutex_lock()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_MUTEX.TRACE_MUTEXLOCK {} 0 bool
addvar TRACE_MUTEXTRYLOCK$i $i {configos.h} u  "1" 5 {} {} {} {} {} {} {} {} {pthread_mutex_trylock()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_MUTEX.TRACE_MUTEXTRYLOCK {} 0 bool
addvar TRACE_MUTEXTIMEDLOCK$i $i {configos.h} u  "1" 6 {} {} {} {} {} {} {} {} {pthread_mutex_timedlock()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_MUTEX.TRACE_MUTEXTIMEDLOCK {} 0 bool
addvar TRACE_MUTEXUNLOCK$i $i {configos.h} u  "1" 7 {} {} {} {} {} {} {} {} {pthread_mutex_unlock} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_MUTEX.TRACE_MUTEXUNLOCK {} 0 bool
addvar TRACE_GLB_COND$i $i {configos.h} u  "0" 11 {} {} {} {} {} {guide/P_config_dial_trace_filters_cond.htm} {} {} {Conditional variable management functions} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_COND {} 0 bool
addvar TRACE_CONDBROADCAST$i $i {configos.h} u  "1" 0 {} {} {} {} {} {} {} {} {pthread_cond_broadcast()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_COND.TRACE_CONDBROADCAST {} 0 bool
addvar TRACE_CONDDESTROY$i $i {configos.h} u  "1" 1 {} {} {} {} {} {} {} {} {pthread_cond_destroy()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_COND.TRACE_CONDDESTROY {} 0 bool
addvar TRACE_CONDINIT$i $i {configos.h} u  "1" 2 {} {} {} {} {} {} {} {} {pthread_cond_init()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_COND.TRACE_CONDINIT {} 0 bool
addvar TRACE_CONDSIGNAL$i $i {configos.h} u  "1" 3 {} {} {} {} {} {} {} {} {pthread_cond_signal()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_COND.TRACE_CONDSIGNAL {} 0 bool
addvar TRACE_CONDTIMEDWAIT$i $i {configos.h} u  "1" 4 {} {} {} {} {} {} {} {} {pthread_cond_timedwait()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_COND.TRACE_CONDTIMEDWAIT {} 0 bool
addvar TRACE_CONDWAIT$i $i {configos.h} u  "1" 5 {} {} {} {} {} {} {} {} {pthread_cond_wait()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_COND.TRACE_CONDWAIT {} 0 bool
addvar TRACE_GLB_MQP$i $i {configos.h} u  "0" 12 {} {} {} {} {} {guide/P_config_dial_trace_filters_mq.htm} {} {} {Message queue management functions} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_MQP {} 0 bool
addvar TRACE_MQCLOSE$i $i {configos.h} u  "1" 0 {} {} {} {} {} {} {} {} {mq_close()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_MQP.TRACE_MQCLOSE {} 0 bool
addvar TRACE_MQNOTIFY$i $i {configos.h} u  "1" 1 {} {} {} {} {} {} {} {} {mq_notify()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_MQP.TRACE_MQNOTIFY {} 0 bool
addvar TRACE_MQOPEN$i $i {configos.h} u  "1" 2 {} {} {} {} {} {} {} {} {mq_open()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_MQP.TRACE_MQOPEN {} 0 bool
addvar TRACE_MQRECEIVE$i $i {configos.h} u  "1" 3 {} {} {} {} {} {} {} {} {mq_receive()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_MQP.TRACE_MQRECEIVE {} 0 bool
addvar TRACE_MQTIMEDRECEIVE$i $i {configos.h} u  "1" 4 {} {} {} {} {} {} {} {} {mq_timedreceive()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_MQP.TRACE_MQTIMEDRECEIVE {} 0 bool
addvar TRACE_MQSEND$i $i {configos.h} u  "1" 5 {} {} {} {} {} {} {} {} {mq_send()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_MQP.TRACE_MQSEND {} 0 bool
addvar TRACE_MQTIMEDSEND$i $i {configos.h} u  "1" 6 {} {} {} {} {} {} {} {} {mq_timedsend()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_MQP.TRACE_MQTIMEDSEND {} 0 bool
addvar TRACE_MQSETATTR$i $i {configos.h} u  "1" 7 {} {} {} {} {} {} {} {} {mq_setattr()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_MQP.TRACE_MQSETATTR {} 0 bool
addvar TRACE_MQGETATTR$i $i {configos.h} u  "1" 8 {} {} {} {} {} {} {} {} {mq_getattr()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_MQP.TRACE_MQGETATTR {} 0 bool
addvar TRACE_MQUNLINK$i $i {configos.h} u  "1" 9 {} {} {} {} {} {} {} {} {mq_unlink()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_MQP.TRACE_MQUNLINK {} 0 bool
addvar TRACE_GLB_TMR$i $i {configos.h} u  "0" 13 {} {} {} {} {} {guide/P_config_dial_trace_filters_tmr.htm} {} {} {Timer management functions} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_TMR {} 0 bool
addvar TRACE_TMRSLEEP$i $i {configos.h} u  "1" 0 {} {} {} {} {} {} {} {} {sleep()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_TMR.TRACE_TMRSLEEP {} 0 bool
addvar TRACE_TMRNANOSLEEP$i $i {configos.h} u  "1" 1 {} {} {} {} {} {} {} {} {nanosleep()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_TMR.TRACE_TMRNANOSLEEP {} 0 bool
addvar TRACE_TMREXPIRE$i $i {configos.h} u  "1" 2 {} {} {} {} {} {} {} {} {timerExpire()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_TMR.TRACE_TMREXPIRE {} 0 bool
addvar TRACE_TMRCREATE$i $i {configos.h} u  "1" 3 {} {} {} {} {} {} {} {} {timer_create()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_TMR.TRACE_TMRCREATE {} 0 bool
addvar TRACE_TMRDELETE$i $i {configos.h} u  "1" 4 {} {} {} {} {} {} {} {} {timer_delete()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_TMR.TRACE_TMRDELETE {} 0 bool
addvar TRACE_TMRGETOVERRUN$i $i {configos.h} u  "1" 5 {} {} {} {} {} {} {} {} {timer_getoverrun()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_TMR.TRACE_TMRGETOVERRUN {} 0 bool
addvar TRACE_TMRSETTIME$i $i {configos.h} u  "1" 6 {} {} {} {} {} {} {} {} {timer_settime()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_TMR.TRACE_TMRSETTIME {} 0 bool
addvar TRACE_TMRCLCSETTIME$i $i {configos.h} u  "1" 7 {} {} {} {} {} {} {} {} {clock_settime()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_TMR.TRACE_TMRCLCSETTIME {} 0 bool
addvar TRACE_GLB_IO$i $i {configos.h} u  "0" 14 {} {} {} {} {} {guide/P_config_dial_trace_filters_io.htm} {} {} {IO management functions} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_IO {} 0 bool
addvar TRACE_IOMKFIFO$i $i {configos.h} u  "1" 0 {} {} {} {} {} {} {} {} {mkfifo()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_IO.TRACE_IOMKFIFO {} 0 bool
addvar TRACE_IOLSEEK$i $i {configos.h} u  "1" 1 {} {} {} {} {} {} {} {} {lseek()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_IO.TRACE_IOLSEEK {} 0 bool
addvar TRACE_IOOPEN$i $i {configos.h} u  "1" 2 {} {} {} {} {} {} {} {} {open(),creat()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_IO.TRACE_IOOPEN {} 0 bool
addvar TRACE_IOCLOSE$i $i {configos.h} u  "1" 3 {} {} {} {} {} {} {} {} {close()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_IO.TRACE_IOCLOSE {} 0 bool
addvar TRACE_IODUP$i $i {configos.h} u  "1" 4 {} {} {} {} {} {} {} {} {dup()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_IO.TRACE_IODUP {} 0 bool
addvar TRACE_IODUP2$i $i {configos.h} u  "1" 5 {} {} {} {} {} {} {} {} {dup2()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_IO.TRACE_IODUP2 {} 0 bool
addvar TRACE_IOPIPE$i $i {configos.h} u  "1" 6 {} {} {} {} {} {} {} {} {pipe()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_IO.TRACE_IOPIPE {} 0 bool
addvar TRACE_IOREAD$i $i {configos.h} u  "1" 7 {} {} {} {} {} {} {} {} {read()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_IO.TRACE_IOREAD {} 0 bool
addvar TRACE_IOWRITE$i $i {configos.h} u  "1" 8 {} {} {} {} {} {} {} {} {write()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_IO.TRACE_IOWRITE {} 0 bool
addvar TRACE_IOFDATASYNC$i $i {configos.h} u  "1" 9 {} {} {} {} {} {} {} {} {fdatasync()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_IO.TRACE_IOFDATASYNC {} 0 bool
addvar TRACE_AIOREAD$i $i {configos.h} u  "1" 10 {} {} {} {} {} {} {} {} {aio_read()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_IO.TRACE_AIOREAD {} 0 bool
addvar TRACE_AIOWRITE$i $i {configos.h} u  "1" 11 {} {} {} {} {} {} {} {} {aio_write()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_IO.TRACE_AIOWRITE {} 0 bool
addvar TRACE_AIOFSYNC$i $i {configos.h} u  "1" 12 {} {} {} {} {} {} {} {} {aio_fsync()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_IO.TRACE_AIOFSYNC {} 0 bool
addvar TRACE_AIOCANCEL$i $i {configos.h} u  "1" 13 {} {} {} {} {} {} {} {} {aio_cancel()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_IO.TRACE_AIOCANCEL {} 0 bool
addvar TRACE_AIOERROR$i $i {configos.h} u  "1" 14 {} {} {} {} {} {} {} {} {aio_error()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_IO.TRACE_AIOERROR {} 0 bool
addvar TRACE_AIORETURN$i $i {configos.h} u  "1" 15 {} {} {} {} {} {} {} {} {aio_return()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_IO.TRACE_AIORETURN {} 0 bool
addvar TRACE_AIOSUSPEND$i $i {configos.h} u  "1" 16 {} {} {} {} {} {} {} {} {aio_suspend()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_IO.TRACE_AIOSUSPEND {} 0 bool
addvar TRACE_MPMEM$i $i {configos.h} u  "0" 15 {$INCLUDE_MULTIPROCESSING==1 && $INCLUDE_MP_OBJS==1} {} {} {} {} {} {} {} {Memory allocation(MP)} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_MPMEM {} 0 bool
addvar TRACE_MPMEMPOOLCREATE$i $i {configos.h} u  "1" 0 {} {} {} {} {} {} {} {} {memPoolCreate()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_MPMEM.TRACE_MPMEMPOOLCREATE {} 0 bool
addvar TRACE_MPMEMPOOLADD$i $i {configos.h} u  "1" 1 {} {} {} {} {} {} {} {} {memPoolAdd()} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_MPMEM.TRACE_MPMEMPOOLADD {} 0 bool
addvar TRACE_MPMEMPOOLALLOC$i $i {configos.h} u  "1" 2 {} {} {} {} {} {} {} {} {Mem alloc} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_MPMEM.TRACE_MPMEMPOOLALLOC {} 0 bool
addvar TRACE_MPMEMPOOLFREE$i $i {configos.h} u  "1" 3 {} {} {} {} {} {} {} {} {Mem free} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_MPMEM.TRACE_MPMEMPOOLFREE {} 0 bool
addvar TRACE_MPSEM$i $i {configos.h} u  "0" 16 {$INCLUDE_MULTIPROCESSING==1 && $INCLUDE_MP_OBJS==1} {} {} {} {} {} {} {} {Semaphore management functions(MP)} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_MPSEM {} 0 bool
addvar TRACE_MPSEMTRYWAIT$i $i {configos.h} u  "1" 0 {} {} {} {} {} {} {} {} {sem_trywait() MP} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_MPSEM.TRACE_MPSEMTRYWAIT {} 0 bool
addvar TRACE_MPSEMTIMEDWAIT$i $i {configos.h} u  "1" 1 {} {} {} {} {} {} {} {} {sem_timedwait MP} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_MPSEM.TRACE_MPSEMTIMEDWAIT {} 0 bool
addvar TRACE_MPSEMWAIT$i $i {configos.h} u  "1" 2 {} {} {} {} {} {} {} {} {sem_wait() MP} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_MPSEM.TRACE_MPSEMWAIT {} 0 bool
addvar TRACE_MPSEMPOST$i $i {configos.h} u  "1" 3 {} {} {} {} {} {} {} {} {sem_post() MP} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_MPSEM.TRACE_MPSEMPOST {} 0 bool
addvar TRACE_MPSEMINIT$i $i {configos.h} u  "1" 4 {} {} {} {} {} {} {} {} {sem_init() MP} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_MPSEM.TRACE_MPSEMINIT {} 0 bool
addvar TRACE_MPSEMOPEN$i $i {configos.h} u  "1" 5 {} {} {} {} {} {} {} {} {sem_open() MP} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_MPSEM.TRACE_MPSEMOPEN {} 0 bool
addvar TRACE_MPSEMGETVALUE$i $i {configos.h} u  "1" 6 {} {} {} {} {} {} {} {} {sem_getvalue() MP} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_MPSEM.TRACE_MPSEMGETVALUE {} 0 bool
addvar TRACE_MPMQ$i $i {configos.h} u  "0" 17 {$INCLUDE_MULTIPROCESSING==1 && $INCLUDE_MP_OBJS==1} {} {} {} {} {} {} {} {Message queue management functions(MP)} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_MPMQ {} 0 bool
addvar TRACE_MPMQOPEN$i $i {configos.h} u  "1" 0 {} {} {} {} {} {} {} {} {mq_open() MP} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_MPMQ.TRACE_MPMQOPEN {} 0 bool
addvar TRACE_MPMQRECEIVE$i $i {configos.h} u  "1" 1 {} {} {} {} {} {} {} {} {mq_receive() MP} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_MPMQ.TRACE_MPMQRECEIVE {} 0 bool
addvar TRACE_MPMQTIMEDRECEIVE$i $i {configos.h} u  "1" 2 {} {} {} {} {} {} {} {} {mq_timedreceive() MP} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_MPMQ.TRACE_MPMQTIMEDRECEIVE {} 0 bool
addvar TRACE_MPMQSEND$i $i {configos.h} u  "1" 3 {} {} {} {} {} {} {} {} {mq_send() MP} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_MPMQ.TRACE_MPMQSEND {} 0 bool
addvar TRACE_MPMQTIMEDSEND$i $i {configos.h} u  "1" 4 {} {} {} {} {} {} {} {} {mq_timedsend() MP} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_MPMQ.TRACE_MPMQTIMEDSEND {} 0 bool
addvar TRACE_MPMQSETATTR$i $i {configos.h} u  "1" 5 {} {} {} {} {} {} {} {} {mq_setattr() MP} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_MPMQ.TRACE_MPMQSETATTR {} 0 bool
addvar TRACE_MPMQGETATTR$i $i {configos.h} u  "1" 6 {} {} {} {} {} {} {} {} {mq_getattr() MP} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_MPMQ.TRACE_MPMQGETATTR {} 0 bool
addvar TRACE_GLB_USEREVENT$i $i {configos.h} u  "1" 18 {} {} {} {} {} {} {} {} {} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_USEREVENT {} 0 bool
addvar TRACE_GLB_USER$i $i {configos.h} u  "0" 0 {} {} {} {} {} {guide/P_config_dial_trace_filters_user.htm} {} {} {User events} OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT.TRACE_GLB_USEREVENT.TRACE_GLB_USER {} 0 bool
addvar INCLUDE_GTK$i $i {usermake.def} u  "0" 1 {$INCLUDE_XCLIENT==1 && $INCLUDE_GBRV==1} {} {} {} {Include GTK library} {} {} {} {GTK} INCLUDE_GTK {} 0 bool
addvar GTK_DIR$i $i {usermake.def} s  "" 0 {} {} {} {} {Path to GTK directory} {} {} {} {GTK directory} INCLUDE_GTK.GTK_DIR {} 0 dn
addvar TEXTDOMAINDIR$i $i {configos.h} s  "/tar/locale" 1 {} {} {} {} {Path to GTK localization directory on target file system} {} {} {} {Locale directory} INCLUDE_GTK.TEXTDOMAINDIR {} 0 text
addvar XLOCALEDIR$i $i {configos.h} s  "/tar/xlocale" 2 {} {} {} {} {Path to Xlib localization directory on target file system} {} {} {} {XLocale directory} INCLUDE_GTK.XLOCALEDIR {} 0 text
addvar XWINDOW_INCLUDE$i $i {usermake.def} u  "1" 2 {} {} {} {} {} {} {} {} {X Window} XWINDOW_INCLUDE {} 0 bool
addvar OC2KWINHOME$i $i {usermake.def} s  "" 0 {} {} {} {[string length $P] == 0 || [file isdirectory $P]==1} {Directory where X client is installed (base directory)} {} {} {} {X client directory} XWINDOW_INCLUDE.OC2KWINHOME {} 0 dn
addvar XSERVERBASE$i $i {usermake.def} s  "" 1 {} {} {} {[string length $P] == 0 || [file isdirectory $P]==1} {Directory where X server is installed (base directory)} {} {} {} {X server directory} XWINDOW_INCLUDE.XSERVERBASE {} 0 dn
addvar INCLUDE_XCLIENT$i $i {usermake.def} u  "0" 2 {$INCLUDE_SYMTBL==1 && $INCLUDE_POSIX_CND==1 && $INCLUDE_NETWORK==1} {INCLUDE_GTK } {} {} {Include X client} {} {} {} {X client} XWINDOW_INCLUDE.INCLUDE_XCLIENT {} 0 bool
addvar INCLUDE_GBRV$i $i {usermake.def} u  "0" 0 {[file isfile $OC2KWINHOME/lib/R4000/libX11.a]==1} {INCLUDE_GTK INCLUDE_BICRV } {} {} {Include  GBRV library} {} {} {} {GBRV library} XWINDOW_INCLUDE.INCLUDE_XCLIENT.INCLUDE_GBRV {} 0 bool
addvar INCLUDE_BICRV$i $i {usermake.def} u  "0" 1 {$INCLUDE_GBRV==1 && [file isfile $OC2KWINHOME/lib/R4000/libXW.a]==1} {} {} {} {Include  BICRV library} {} {} {} {BICRV library} XWINDOW_INCLUDE.INCLUDE_XCLIENT.INCLUDE_BICRV {} 0 bool
addvar ENVDISPLAY_SET$i $i {configos.h} u  "0" 2 {$INCLUDE_ENVIRONMENT==1} {ENVDISPLAY_VALUE } {} {} {} {} {} {} {Set DISPLAY variable} XWINDOW_INCLUDE.INCLUDE_XCLIENT.ENVDISPLAY_SET {} 0 bool
addvar ENVDISPLAY_VALUE$i $i {configos.h} s  "127.0.0.1:0.0" 3 {$ENVDISPLAY_SET==1} {} {} {} {Server name or IP address and display number} {} {} {} {Value of DISPLAY variable} XWINDOW_INCLUDE.INCLUDE_XCLIENT.ENVDISPLAY_VALUE {} 0 text
addvar INCLUDE_XSERVER$i $i {usermake.def} u  "0" 3 {$INCLUDE_POSIX_CND==1 && $INCLUDE_NETWORK==1 && [file isfile $XSERVERBASE/x_txt/startx42.c]==1} {} {} {} {Include X server} {} {} {} {X server} XWINDOW_INCLUDE.INCLUDE_XSERVER {} 0 bool
addvar XSERVER_START$i $i {configos.h} u  "0" 0 {} {XSERVER_PRIORITY } {} {} {Start thread executing X server} {} {} {} {Start X server} XWINDOW_INCLUDE.INCLUDE_XSERVER.XSERVER_START {} 0 bool
addvar XSERVER_PRIORITY$i $i {configos.h} i  "100" 1 {$XSERVER_START==1} {} {} {$P >= 0 && $P <= 255} {The value for X server thread priority} {} {} {} {X server priority} XWINDOW_INCLUDE.INCLUDE_XSERVER.XSERVER_PRIORITY {} 0 int
addvar XSERVER_NAME$i $i {usermake.def} i  "XF86_ogk" 2 {} {} {} { [string length $P] == 0 || [file isfile $XSERVERBASE/x_obj/$P.o]==1} {Graphic adapter type} {} {} {} {X server type} XWINDOW_INCLUDE.INCLUDE_XSERVER.XSERVER_NAME { {XF86_ogk} } 0 combo
addvar XSERVER_HOSTSCNT$i $i {configos.h} u  "0" 3 {} {} {} {} {Enable host-based access control} {} {} {} {Access control} XWINDOW_INCLUDE.INCLUDE_XSERVER.XSERVER_HOSTSCNT {} 0 bool
addvar XSERVER_PARMCONFIG$i $i {configos.h} u  "1" 4 {} {XSERVER_PREFIX XSERVER_RGB XSERVER_FONTS XSERVER_DEPTH XSERVER_MODES } {} {} {Parameters below will overload config file parameters} {} {} {} {Overload config param.} XWINDOW_INCLUDE.INCLUDE_XSERVER.XSERVER_PARMCONFIG {} 0 bool
addvar XSERVER_PREFIX$i $i {configos.h} s  "/tar" 5 {$XSERVER_PARMCONFIG==1} {} {} {} {Prefix for RGB color database, font pathes & config file name} {} {} {} {Prefix} XWINDOW_INCLUDE.INCLUDE_XSERVER.XSERVER_PREFIX {} 0 text
addvar XSERVER_RGB$i $i {configos.h} s  "/X/rgb" 6 {$XSERVER_PARMCONFIG==1} {} {} {} {Path name for the RGB color database} {} {} {} {RGB path} XWINDOW_INCLUDE.INCLUDE_XSERVER.XSERVER_RGB {} 0 text
addvar XSERVER_FONTS$i $i {configos.h} s  "/X/fonts/besta,/X/fonts/cyrillic,/X/fonts/misc" 7 {$XSERVER_PARMCONFIG==1} {} {} {} {Search path for fonts} {} {} {} {Font pathes} XWINDOW_INCLUDE.INCLUDE_XSERVER.XSERVER_FONTS {} 0 text
addvar XSERVER_DEPTH$i $i {configos.h} i  "8" 8 {$XSERVER_PARMCONFIG==1} {} {} {} {Color depth (may be 8, or 16, or 32)} {} {} {} {Depth} XWINDOW_INCLUDE.INCLUDE_XSERVER.XSERVER_DEPTH {} 0 int
addvar XSERVER_MODES$i $i {configos.h} s  "1024x768,800x600,640x480" 9 {$XSERVER_PARMCONFIG==1} {} {} {} {List of video modes to use} {} {} {} {Modes} XWINDOW_INCLUDE.INCLUDE_XSERVER.XSERVER_MODES {} 0 text
addvar XSERVER_CFGTYPE$i $i {usermake.def} u  "1" 10 {$INCLUDE_TAR_FS==1} {XSERVER_CFGFILE } {} {} {Include X server config file in the OS image} {} {} {} {Include config in the image} XWINDOW_INCLUDE.INCLUDE_XSERVER.XSERVER_CFGTYPE {} 0 bool
addvar XSERVERCFGPATH$i $i {usermake.def} s  "" 0 {} {} {} {[string length $P] == 0 || [file isdirectory $P]==1} {Directory where X server config is installed} {} {} {} {X server config directory} XWINDOW_INCLUDE.INCLUDE_XSERVER.XSERVER_CFGTYPE.XSERVERCFGPATH {} 0 dn
addvar XSERVER_CFGFILE$i $i {configos.h} s  "/mips/XF86Config" 11 {$XSERVER_CFGTYPE==0} {} {} {} {X server config file (path)name} {} {} {} {XF86Config file} XWINDOW_INCLUDE.INCLUDE_XSERVER.XSERVER_CFGFILE {} 0 text
addvar XSERVER_OUT$i $i {configos.h} s  "" 12 {} {} {} {} {File name for server output messages (console by default)} {} {} {} {X server out stream} XWINDOW_INCLUDE.INCLUDE_XSERVER.XSERVER_OUT {} 0 text
addvar XSERVER_LOG$i $i {configos.h} u  "0" 13 {} {XSERVER_LOGFILE } {} {} {Enable X server system log} {} {} {} {X server log} XWINDOW_INCLUDE.INCLUDE_XSERVER.XSERVER_LOG {} 0 bool
addvar XSERVER_LOGFILE$i $i {configos.h} s  "/nfs/syslog" 14 {$XSERVER_LOG==1} {} {} {} {X server system log file (path)name} {} {} {} {Log file name} XWINDOW_INCLUDE.INCLUDE_XSERVER.XSERVER_LOGFILE {} 0 text
addfile "configos.h"
addfile "usermake.def"
proc gentree {ns} {
if !$::x_enabled return
global tree h img1 img2 img3
$h add "OC2000_CONFIG" -itemtype imagetext -text "[getname OC2000_CONFIG]" -image $img2 -data ${ns}::dialog_OC2000_CONFIG 
$h add "OC2000_CONFIG.OC2000_CONFIG_KERNEL" -itemtype imagetext -text "[getname OC2000_CONFIG_KERNEL]" -image $img1 -data ${ns}::dialog_OC2000_CONFIG_KERNEL 
$h add "OC2000_CONFIG.OC2000_CONFIG_SCALING" -itemtype imagetext -text "[getname OC2000_CONFIG_SCALING]" -image $img1 -data ${ns}::dialog_OC2000_CONFIG_SCALING 
$h add "OC2000_CONFIG.OC2000_CONFIG_A" -itemtype imagetext -text "[getname OC2000_CONFIG_A]" -image $img1 -data ${ns}::dialog_OC2000_CONFIG_A 
$h add "OC2000_CONFIG.INCLUDE_NETWORK" -itemtype imagetext -text "[getname INCLUDE_NETWORK]" -image $img1 -data ${ns}::dialog_INCLUDE_NETWORK 
$h add "OC2000_CONFIG.INCLUDE_NETWORK.CONFIG_NETADDRESSES" -itemtype imagetext -text "[getname CONFIG_NETADDRESSES]" -image $img1 -data ${ns}::dialog_CONFIG_NETADDRESSES 
$h add "OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_HOST_TABLE_INIT" -itemtype imagetext -text "[getname INCLUDE_HOST_TABLE_INIT]" -image $img1 -data ${ns}::dialog_INCLUDE_HOST_TABLE_INIT 
$h add "OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_NETGW" -itemtype imagetext -text "[getname INCLUDE_NETGW]" -image $img1 -data ${ns}::dialog_INCLUDE_NETGW
$h add "OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_SNETGW" -itemtype imagetext -text "[getname INCLUDE_SNETGW]" -image $img1 -data ${ns}::dialog_INCLUDE_SNETGW
$h add "OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_SLIP" -itemtype imagetext -text "[getname INCLUDE_SLIP]" -image $img1 -data ${ns}::dialog_INCLUDE_SLIP
$h add "OC2000_CONFIG.INCLUDE_NETWORK.INCLUDE_PPP" -itemtype imagetext -text "[getname INCLUDE_PPP]" -image $img1 -data ${ns}::dialog_INCLUDE_PPP
$h add "OC2000_CONFIG.INCLUDE_NETWORK.MBUF_CONFIG" -itemtype imagetext -text "[getname MBUF_CONFIG]" -image $img1 -data ${ns}::dialog_MBUF_CONFIG 
$h add "OC2000_CONFIG.INCLUDE_EXTRA_FS" -itemtype imagetext -text "[getname INCLUDE_EXTRA_FS]" -image $img1 -data ${ns}::dialog_INCLUDE_EXTRA_FS 
$h add "OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_VFAT" -itemtype imagetext -text "[getname MOUNT_VFAT]" -image $img1 -data ${ns}::dialog_MOUNT_VFAT 
$h add "OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_CD9660" -itemtype imagetext -text "[getname MOUNT_CD9660]" -image $img1 -data ${ns}::dialog_MOUNT_CD9660
$h add "OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_TAR" -itemtype imagetext -text "[getname MOUNT_TAR]" -image $img1 -data ${ns}::dialog_MOUNT_TAR
$h add "OC2000_CONFIG.INCLUDE_EXTRA_FS.INCLUDE_TARFILES" -itemtype imagetext -text "[getname INCLUDE_TARFILES]" -image $img1 -data ${ns}::dialog_INCLUDE_TARFILES
$h add "OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_NFS_AUTO" -itemtype imagetext -text "[getname MOUNT_NFS_AUTO]" -image $img1 -data ${ns}::dialog_MOUNT_NFS_AUTO 
$h add "OC2000_CONFIG.INCLUDE_EXTRA_FS.MOUNT_FTP" -itemtype imagetext -text "[getname MOUNT_FTP]" -image $img1 -data ${ns}::dialog_MOUNT_FTP
$h add "OC2000_CONFIG.INCLUDE_MULTIPROCESSING" -itemtype imagetext -text "[getname INCLUDE_MULTIPROCESSING]" -image $img1 -data ${ns}::dialog_INCLUDE_MULTIPROCESSING 
$h add "OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS" -itemtype imagetext -text "[getname INCLUDE_CHANNELS]" -image $img1 -data ${ns}::dialog_INCLUDE_CHANNELS 
$h add "OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.INCLUDE_ARINC_QPORT" -itemtype imagetext -text "[getname INCLUDE_ARINC_QPORT]" -image $img1 -data ${ns}::dialog_INCLUDE_ARINC_QPORT 
$h add "OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.INCLUDE_ARINC_SPORT" -itemtype imagetext -text "[getname INCLUDE_ARINC_SPORT]" -image $img1 -data ${ns}::dialog_INCLUDE_ARINC_SPORT 
$h add "OC2000_CONFIG.INCLUDE_MULTIPROCESSING.INCLUDE_CHANNELS.CONNECTION_CONFIG" -itemtype imagetext -text "[getname CONNECTION_CONFIG]" -image $img1 -data ${ns}::dialog_CONNECTION_CONFIG
$h add "OC2000_CONFIG.OC2000_MISC" -itemtype imagetext -text "[getname OC2000_MISC]" -image $img1 -data ${ns}::dialog_OC2000_MISC 
$h add "OC2000_CONFIG.OC2000_APPLICATION" -itemtype imagetext -text "[getname OC2000_APPLICATION]" -image $img1 -data ${ns}::dialog_OC2000_APPLICATION 
$h add "OC2000_CONFIG.OC2000_CONFIG_D" -itemtype imagetext -text "[getname OC2000_CONFIG_D]" -image $img1 -data ${ns}::dialog_OC2000_CONFIG_D 
$h add "OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SYSLOG" -itemtype imagetext -text "[getname INCLUDE_SYSLOG]" -image $img1 -data ${ns}::dialog_INCLUDE_SYSLOG 
$h add "OC2000_CONFIG.OC2000_CONFIG_D.REPORT_ERRORS" -itemtype imagetext -text "[getname REPORT_ERRORS]" -image $img1 -data ${ns}::dialog_REPORT_ERRORS 
$h add "OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_PRSMEM" -itemtype imagetext -text "[getname INCLUDE_PRSMEM]" -image $img1 -data ${ns}::dialog_INCLUDE_PRSMEM 
$h add "OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_FLASHMEM" -itemtype imagetext -text "[getname INCLUDE_FLASHMEM]" -image $img1 -data ${ns}::dialog_INCLUDE_FLASHMEM 
$h add "OC2000_CONFIG.OC2000_CONFIG_D.INCLUDE_SYSSHELL" -itemtype imagetext -text "[getname INCLUDE_SYSSHELL]" -image $img1 -data ${ns}::dialog_INCLUDE_SYSSHELL 
$h add "OC2000_CONFIG.INCLUDE_TRACE" -itemtype imagetext -text "[getname INCLUDE_TRACE]" -image $img1 -data ${ns}::dialog_INCLUDE_TRACE 
$h add "OC2000_CONFIG.INCLUDE_TRACE.CONFIG_TRACE_EXCEPTION" -itemtype imagetext -text "[getname CONFIG_TRACE_EXCEPTION]" -image $img1 -data ${ns}::dialog_CONFIG_TRACE_EXCEPTION 
$h hide entry "OC2000_CONFIG.INCLUDE_TRACE.CONFIG_TRACE_EXCEPTION"
$h add "OC2000_CONFIG.INCLUDE_TRACE.TRACE_CHANNEL_CONFIG" -itemtype imagetext -text "[getname TRACE_CHANNEL_CONFIG]" -image $img1 -data ${ns}::dialog_TRACE_CHANNEL_CONFIG
$h hide entry "OC2000_CONFIG.INCLUDE_TRACE.TRACE_CHANNEL_CONFIG"
$h add "OC2000_CONFIG.INCLUDE_TRACE.TRACE_LOAD_CONFIG" -itemtype imagetext -text "[getname TRACE_LOAD_CONFIG]" -image $img1 -data ${ns}::dialog_TRACE_LOAD_CONFIG 
$h hide entry "OC2000_CONFIG.INCLUDE_TRACE.TRACE_LOAD_CONFIG"
$h add "OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT" -itemtype imagetext -text "[getname TRACE_EVENT]" -image $img1 -data ${ns}::dialog_TRACE_EVENT 
$h hide entry "OC2000_CONFIG.INCLUDE_TRACE.TRACE_EVENT"
$h add "INCLUDE_GTK" -itemtype imagetext -text "[getname INCLUDE_GTK]" -image $img2 -data ${ns}::dialog_INCLUDE_GTK 
$h add "XWINDOW_INCLUDE" -itemtype imagetext -text "[getname XWINDOW_INCLUDE]" -image $img2 -data ${ns}::dialog_XWINDOW_INCLUDE 
$tree autosetmode
$h selection set OC2000_CONFIG
}
x_startup {OC2000_CONFIG}
set es [extern_sources]
user_setvar
gentree {}
foreach i $es { ${i}::external_gentree $i }
if [file exists project.prj] "loadproject project.prj"
