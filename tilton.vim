" Vim syntax file
" Language:    Tilton
" Maintainer:  JR <jr@reveluxlabs.com
" URL:         http://github.com/reveluxlabs/Tilton
" License:     MIT

" ---------------------------------------------------------------------
"  Load Once: {{{1
" For vim-version 5.x: Clear all syntax items
" For vim-version 6.x: Quit when a syntax file was already loaded
if version < 600
  syntax clear
elseif exists("b:current_syntax")
  finish
endif

" ---------------------------------------------------------------------
" Clusters: {{{1
syn cluster tiltonBracketGroup contains=tiltonBuiltin,tiltonNumber,tiltonSpecial,tiltonSymbol,tiltonVar 

" ---------------------------------------------------------------------
" Macros: {{{1
syn region  tiltonBracket         transparent matchgroup=hlLevel0 start='<\~' end='\~>' contains=@tiltonBracketGroup,tiltonBracket1
syn region  tiltonBracket1        transparent matchgroup=hlLevel1 start='<\~' end='\~>' contains=@tiltonBracketGroup,tiltonBracket2
syn region  tiltonBracket2        transparent matchgroup=hlLevel2 start='<\~' end='\~>' contains=@tiltonBracketGroup,tiltonBracket3
syn region  tiltonBracket3        transparent matchgroup=hlLevel3 start='<\~' end='\~>' contains=@tiltonBracketGroup,tiltonBracket4
syn region  tiltonBracket4        transparent matchgroup=hlLevel4 start='<\~' end='\~>' contains=@tiltonBracketGroup,tiltonBracket5
syn region  tiltonBracket5        transparent matchgroup=hlLevel5 start='<\~' end='\~>' contains=@tiltonBracketGroup,tiltonBracket6
syn region  tiltonBracket6        transparent matchgroup=hlLevel6 start='<\~' end='\~>' contains=@tiltonBracketGroup,tiltonBracket7
syn region  tiltonBracket7        transparent matchgroup=hlLevel7 start='<\~' end='\~>' contains=@tiltonBracketGroup,tiltonBracket8
syn region  tiltonBracket8        transparent matchgroup=hlLevel8 start='<\~' end='\~>' contains=@tiltonBracketGroup,tiltonBracket9
syn region  tiltonBracket9        transparent matchgroup=hlLevel9 start='<\~' end='\~>' contains=@tiltonBracketGroup,tiltonBracket

" ---------------------------------------------------------------------
" Standard Tilton Builtins and Macros: {{{1
syn keyword tiltonBuiltin contained  add          gt           number?
syn keyword tiltonBuiltin contained  and          gt?          or
syn keyword tiltonBuiltin contained  append       include      print
syn keyword tiltonBuiltin contained  define       last         read
syn keyword tiltonBuiltin contained  defined?     le?          rep
syn keyword tiltonBuiltin contained  delete       length       set
syn keyword tiltonBuiltin contained  div          literal      slashify
syn keyword tiltonBuiltin contained  dump         loop         stop
syn keyword tiltonBuiltin contained  entityify    lt           sub
syn keyword tiltonBuiltin contained  eq?          lt?          substr
syn keyword tiltonBuiltin contained  eval         mod          tilde
syn keyword tiltonBuiltin contained  first        mult         tilton
syn keyword tiltonBuiltin contained  ge?          mute         trim
syn keyword tiltonBuiltin contained  gensym       ne?          unicode
syn keyword tiltonBuiltin contained  get          null         write

" ---------------------------------------------------------------------
" Standard Tilton Variables: {{{1
syn keyword tiltonVar		!<\~\d\~>!

" ---------------------------------------------------------------------
" Numbers: supporting integers and floating point numbers {{{1
syn match tiltonNumber		"-\=\(\.\d\+\|\d\+\(\.\d*\)\=\)\([dDeEfFlL][-+]\=\d\+\)\="
syn match tiltonNumber		"-\=\(\d\+/\d\+\)"

syn match tiltonSymbol contained ![^<~>]]!
syn match tiltonSpecial		!\~!

syn match tiltonBracketError '\~>'

" ---------------------------------------------------------------------
" Synchronization: {{{1
syn sync lines=100

" ---------------------------------------------------------------------
" Define Highlighting: {{{1
hi def link tiltonBuiltin       Statement
hi def link tiltonBracketError  Error
"hi def link tiltonNumber        Number
hi def link tiltonSpecial       Type
hi def link tiltonSymbol        Identifier
hi def link tiltonVar           Statement


if &bg == "dark"
    hi default   hlLevel0 ctermfg=red         guifg=red1
    hi default   hlLevel1 ctermfg=yellow      guifg=orange1      
    hi default   hlLevel2 ctermfg=green       guifg=yellow1      
    hi default   hlLevel3 ctermfg=cyan        guifg=greenyellow  
    hi default   hlLevel4 ctermfg=magenta     guifg=green1       
    hi default   hlLevel5 ctermfg=red         guifg=springgreen1 
    hi default   hlLevel6 ctermfg=yellow      guifg=cyan1        
    hi default   hlLevel7 ctermfg=green       guifg=slateblue1   
    hi default   hlLevel8 ctermfg=cyan        guifg=magenta1     
    hi default   hlLevel9 ctermfg=magenta     guifg=purple1
else
    hi default   hlLevel0 ctermfg=red         guifg=red3
    hi default   hlLevel1 ctermfg=darkyellow  guifg=orangered3
    hi default   hlLevel2 ctermfg=darkgreen   guifg=orange2
    hi default   hlLevel3 ctermfg=blue        guifg=yellow3
    hi default   hlLevel4 ctermfg=darkmagenta guifg=olivedrab4
    hi default   hlLevel5 ctermfg=red         guifg=green4
    hi default   hlLevel6 ctermfg=darkyellow  guifg=paleturquoise3
    hi default   hlLevel7 ctermfg=darkgreen   guifg=deepskyblue4
    hi default   hlLevel8 ctermfg=blue        guifg=darkslateblue
    hi default   hlLevel9 ctermfg=darkmagenta guifg=darkviolet
endif

let b:current_syntax = "tilton"
" ---------------------------------------------------------------------
" vim: ts=2 nowrap fdm=marker
