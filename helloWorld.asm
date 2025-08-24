; "Hello world!" program to test LCD display
; designed to work with 1602 16x2 char LCD display, but also works in Logisim

; memory mapped output
LCD_char = 0xff
LCD_inst = 0xfe

start:
	
	; initialize the zero and temp variables to 0
	subleq zero zero
	subleq temp temp

	; set the target for the LCD initialization loop
	subleq init_loop init_loop
	subleq init_init zero
	subleq zero init_loop
	subleq zero zero

	; set the target for the string printing loop
	subleq loop loop
	subleq string_init zero
	subleq zero loop
	subleq zero zero

; loop to initialize the LCD display
init_loop:
	subleq init_sequence zero
	subleq LCD_inst LCD_inst
	subleq zero LCD_inst loop
	subleq zero zero
	
	subleq minus_one init_loop
	subleq zero zero init_loop

; loop to print the "Hello world!" string
loop:
	subleq string zero			; zero = -char
	subleq zero temp end
	subleq temp temp
	subleq LCD_char temp		; temp = -LCD_char
	subleq temp zero			; zero = LCD_char - char
	subleq zero LCD_char		; LCD_char = LCD_char - (LCD_char - char)
	subleq zero zero

	subleq minus_one loop
	subleq temp temp loop

end:
	subleq zero zero end

; the "Hello World!" string
string:
	.byte 0x48
	.byte 0x65
	.byte 0x6C
	.byte 0x6C
	.byte 0x6F
	.byte 0x20
	.byte 0x77
	.byte 0x6F
	.byte 0x72
	.byte 0x6C
	.byte 0x64
	.byte 0x21
	.byte 0x00

; LCD initialization sequence
init_sequence:
	.byte 0x38
	.byte 0x0C
	.byte 0x01
	.byte 0x06
	.byte 0x00

; data
init_init:		.byte init_sequence
string_init:	.byte string
zero:			.byte 0x00
one:			.byte 0x01
minus_one:		.byte 0xFF
temp:			.byte 0x00