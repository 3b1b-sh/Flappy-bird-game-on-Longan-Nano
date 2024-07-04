.section .text
.globl display_splash_screen
.type display_splash_screen, @function

display_splash_screen:
   # save the color parameter in t0
    mv t0, a0
    addi sp, sp, -24 
    sw ra, 20(sp)    


    li a0, 16       
    li a1, 10       
    la a2, msg1
    mv a3, t0     
    jal LCD_ShowString

    li a0, 24
    li a1, 30 
    la a2, msg2
    mv a3, t0
    jal LCD_ShowString

    li a0, 24
    li a1, 50
    la a2, msg3
    mv a3, t0
    jal LCD_ShowString

    li a0, 20
    li a1, 100
    la a2, msg4
    mv a3, t0
    jal LCD_ShowString

    li a0, 4
    li a1, 120
    la a2, msg5
    mv a3, t0
    jal LCD_ShowString

    li a0, 8
    li a1, 140
    la a2, msg6
    mv a3, t0
    jal LCD_ShowString



# choose color according to the input
    li a1, 0xFFFF  
    beq t0, a1, return_0  
    li a1, 0xFFE0   
    beq t0, a1, return_1  

    li a0, 0
    j exit

return_0:
    li a0, 0  
    j exit

return_1:
    li a0, 1  
    j exit

exit:
    lw ra, 20(sp)    
    addi sp, sp, 24  
    ret

.section .rodata
msg1:
    .string "Flappy"
msg2:
    .string "bird"
msg3:
    .string "game"
msg4:
    .string "Press"
msg5:
    .string "key right"
msg6:
    .string "to start"
	