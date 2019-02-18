	mov ax, base
        add ax, 120h
        mov ss, ax
	mov sp, 1000h

        mov ax, data           ; Set data segment to safe? area of memory
        mov ds, ax
	
	call clear

	call load_strings
	mov si, 0h ;promt
	call print
	mov di, 30h ;Q
	

start:
	mov ah, 0
	int 16h
	cmp al, 8
	je back
	cmp al, 13
	je deal_nl
	call print_char
	call save_char
	jmp start
deal_nl:
	call print_string
	mov di, 30h
	mov si, 10h ;newline
	call print
	mov si, 0h ;prompt
	call print
	jmp start
back:
	dec di
	call print_char
	jmp start
	
data: db 00h, 20h
base: db 6h, 66h

print: 
	mov ah, 0eh
.loop:
	lodsb
	cmp al, 0
	je .done
	int 10h
	jmp .loop
.done:
	ret

print_char:
	mov ah, 0eh
	int 10h
	ret

save_char:
	mov [di], al
	inc di
	mov al, 0
	mov [di], al
	ret

print_string:
	mov si, 10h ;newline
	call print
	mov si, 20h ;Prog:
	call print
	mov si, 30h ;Q
ps_loop:
	mov al, [si]
	cmp al, 0
	je ps_done
	cmp al, 32
	je space
	call print_char
	jmp ps_next
space:
	mov di, si
	mov si, 10h ;newline
	call print
	mov si, 27h ;Args:
	call print
	mov si, di
	jmp ps_next
ps_next:
	inc si
	jmp ps_loop
ps_done:
	ret

clear: 
	mov ah, 0h
	int 10h
	ret

load_strings:
	mov di, 0h
	mov byte [di], 'F'
	inc di
	mov byte [di], 'l'
	inc di
	mov byte [di], 'o'
	inc di
	mov byte [di], 'p'
	inc di
	mov byte [di], 'p'
	inc di
	mov byte [di], 'y'
	inc di
	mov byte [di], 'O'
	inc di
	mov byte [di], 'S'
	inc di
	mov byte [di], '#'
	inc di
	mov byte [di], 0
	
;NEWLINE 
	mov di, 10h 
	mov byte [di], 10
	inc di
	mov byte [di], 13
	inc di
	mov byte [di], 0
;PROG:
	mov di, 20h
	mov byte [di], 'P'
	inc di
	mov byte [di], 'r'
	inc di
	mov byte [di], 'o'
	inc di
	mov byte [di], 'g'
	inc di
	mov byte [di], ':'
	inc di
	mov byte [di], ' '
	inc di
	mov byte [di], 0
	inc di
	
;ARGS	
	mov byte [di], 'A'
	inc di
	mov byte [di], 'r'
	inc di
	mov byte [di], 'g'
	inc di
	mov byte [di], 's'
	inc di
	mov byte [di], ':'
	inc di
	mov byte [di], ' '
	inc di
	mov byte [di], 0
ret
