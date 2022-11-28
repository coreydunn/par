" Vim syntax file
" Language: Bob

if exists("b:current_syntax")
	finish
endif

"syntax match BobIdent	/[\a_]\+[\a\d_]*/
syntax match BobComment	/#.*$/
syntax match BobKeyword	/\(for\|while\|if\|ret\|let\|fn\|true\|false\)/
syntax match BobInt		/\d\+/
syntax match BobFloat	/\d\+\.\d\+/
syntax match BobString	/"[^"]*"/

"highlight BobIdent		ctermfg=brown guifg=#ff7700
highlight BobComment	ctermfg=gray guifg=#ff0000
highlight BobKeyword	ctermfg=yellow guifg=#ffff00
highlight BobInt		ctermfg=darkblue guifg=#0000ee
highlight BobFloat		ctermfg=darkred guifg=#ee0000
highlight BobString		ctermfg=brown guifg=#ff7700
