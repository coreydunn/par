" Vim syntax file
" Language: Par

if exists("b:current_syntax")
	finish
endif

"syntax match ParIdent		/[\a_]\+[\a\d_]*/
syntax match ParComment		/#.*$/
syntax keyword ParKeyword	for while if ret let fn true false call
syntax match ParInt			/-\?\d\+/
syntax match ParFloat		/-\?\d\+\.\d\+/
syntax match ParString		/"[^"]*"/

"highlight ParIdent		ctermfg=brown guifg=#ff7700
highlight ParComment	ctermfg=gray guifg=#ff0000
highlight ParKeyword	ctermfg=yellow guifg=#ffff00
highlight ParInt		ctermfg=darkblue guifg=#0000ee
highlight ParFloat		ctermfg=darkred guifg=#ee0000
highlight ParString		ctermfg=brown guifg=#ff7700
