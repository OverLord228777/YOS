; Простой загрузчик ОС
bits 16
org 0x7C00

start:
    ; Настройка сегментов
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Очистка экрана
    mov ax, 0x0003
    int 0x10

    ; Сообщение о загрузке
    mov si, msg_loading
    call print_string

    ; Загрузка ядра с диска
    mov ah, 0x02        ; Функция чтения диска
    mov al, 10          ; Количество секторов для чтения (ядро)
    mov ch, 0           ; Цилиндр
    mov cl, 2           ; Сектор (после загрузчика)
    mov dh, 0           ; Головка
    mov bx, 0x7E00      ; Адрес загрузки ядра
    int 0x13

    ; Переход в защищенный режим
    cli
    lgdt [gdt_descriptor]
    
    ; Включение A20 линии
    in al, 0x92
    or al, 2
    out 0x92, al
    
    ; Установка бита защищенного режима
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    
    ; Дальний прыжок для обновления CS
    jmp 0x08:protected_mode

bits 32
protected_mode:
    ; Настройка сегментов защищенного режима
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000
    
    ; Вызов ядра
    call 0x7E00
    jmp $

; Функции для реального режима
bits 16
print_string:
    mov ah, 0x0E
.print_loop:
    lodsb
    test al, al
    jz .done
    int 0x10
    jmp .print_loop
.done:
    ret

; Данные
msg_loading db "Loading YOS...", 0xD, 0xA, 0

; GDT (Global Descriptor Table)
gdt_start:
    gdt_null:
        dd 0
        dd 0
    
    gdt_code:
        dw 0xFFFF       ; Limit
        dw 0x0000       ; Base
        db 0x00         ; Base
        db 10011010b    ; Access byte
        db 11001111b    ; Flags + Limit
        db 0x00         ; Base
    
    gdt_data:
        dw 0xFFFF       ; Limit
        dw 0x0000       ; Base
        db 0x00         ; Base
        db 10010010b    ; Access byte
        db 11001111b    ; Flags + Limit
        db 0x00         ; Base
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

; Завершение загрузочного сектора
times 510-($-$$) db 0
dw 0xAA55