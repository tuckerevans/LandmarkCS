	bits 16
	org 7c00h
	mov si, loading
	call print
	
	mov ax, 7d00h
	mov es, ax
	mov bx, $0
	
	mov dl, 0
	mov dh, 0
	mov ch, 0
	mov cl, 2
	mov al, 1
	mov ah, 2h
	
	int 13h
	mov si, loaded
	call print	

	jmp 7d00h

	
	
loading: db 'Loading OS into RAM...', 10, 13, 0
loaded: db 'Jumping to OS...', 10, 13, 10, 13, 0

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
