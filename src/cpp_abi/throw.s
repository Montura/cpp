	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 15	sdk_version 10, 15, 6
	.globl	__Z5raisev              ## -- Begin function _Z5raisev
	.p2align	4, 0x90
__Z5raisev:                             ## @_Z5raisev
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movl	$1, %edi
	callq	___cxa_allocate_exception
	movq	__ZTI9Exception@GOTPCREL(%rip), %rsi
	xorl	%ecx, %ecx
	movl	%ecx, %edx
	movq	%rax, %rdi
	callq	___cxa_throw
	.cfi_endproc
                                        ## -- End function
	.globl	__Z18try_but_dont_catchv ## -- Begin function _Z18try_but_dont_catchv
	.p2align	4, 0x90
__Z18try_but_dont_catchv:               ## @_Z18try_but_dont_catchv
Lfunc_begin0:
	.cfi_startproc
	.cfi_personality 155, ___gxx_personality_v0
	.cfi_lsda 16, Lexception0
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
Ltmp0:
	leaq	L_.str(%rip), %rdi
	xorl	%eax, %eax
                                        ## kill: def $al killed $al killed $eax
	callq	_printf
Ltmp1:
	jmp	LBB1_1
LBB1_1:
	jmp	LBB1_6
LBB1_2:
Ltmp2:
                                        ## kill: def $edx killed $edx killed $rdx
	movq	%rax, -8(%rbp)
	movl	%edx, -12(%rbp)
## %bb.3:
	movl	-12(%rbp), %eax
	movl	$1, %ecx
	cmpl	%ecx, %eax
	jne	LBB1_17
## %bb.4:
	movq	-8(%rbp), %rdi
	callq	___cxa_begin_catch
	movq	%rax, -24(%rbp)
Ltmp3:
	leaq	L_.str.1(%rip), %rdi
	xorl	%ecx, %ecx
                                        ## kill: def $cl killed $cl killed $ecx
	movb	%cl, %al
	callq	_printf
Ltmp4:
	jmp	LBB1_5
LBB1_5:
	callq	___cxa_end_catch
LBB1_6:
Ltmp8:
	callq	__Z5raisev
Ltmp9:
	jmp	LBB1_7
LBB1_7:
	jmp	LBB1_14
LBB1_8:
Ltmp5:
                                        ## kill: def $edx killed $edx killed $rdx
	movq	%rax, -8(%rbp)
	movl	%edx, -12(%rbp)
Ltmp6:
	callq	___cxa_end_catch
Ltmp7:
	jmp	LBB1_9
LBB1_9:
	jmp	LBB1_17
LBB1_10:
Ltmp10:
                                        ## kill: def $edx killed $edx killed $rdx
	movq	%rax, -8(%rbp)
	movl	%edx, -12(%rbp)
## %bb.11:
	movl	-12(%rbp), %eax
	movl	$1, %ecx
	cmpl	%ecx, %eax
	jne	LBB1_17
## %bb.12:
	movq	-8(%rbp), %rdi
	callq	___cxa_begin_catch
	movq	%rax, -32(%rbp)
Ltmp11:
	leaq	L_.str.2(%rip), %rdi
	xorl	%ecx, %ecx
                                        ## kill: def $cl killed $cl killed $ecx
	movb	%cl, %al
	callq	_printf
Ltmp12:
	jmp	LBB1_13
LBB1_13:
	callq	___cxa_end_catch
LBB1_14:
	leaq	L_.str.3(%rip), %rdi
	movb	$0, %al
	callq	_printf
	addq	$48, %rsp
	popq	%rbp
	retq
LBB1_15:
Ltmp13:
                                        ## kill: def $edx killed $edx killed $rdx
	movq	%rax, -8(%rbp)
	movl	%edx, -12(%rbp)
Ltmp14:
	callq	___cxa_end_catch
Ltmp15:
	jmp	LBB1_16
LBB1_16:
	jmp	LBB1_17
LBB1_17:
	movq	-8(%rbp), %rdi
	callq	__Unwind_Resume
	ud2
LBB1_18:
Ltmp16:
	movq	%rax, %rdi
	movq	%rdx, -40(%rbp)         ## 8-byte Spill
	callq	___clang_call_terminate
Lfunc_end0:
	.cfi_endproc
	.section	__TEXT,__gcc_except_tab
	.p2align	2
GCC_except_table1:
Lexception0:                        ## ---- LSDA header section -----
	.byte	255                     ## @LPStart Encoding = omit
	.byte	155                     ## @TType Encoding = indirect pcrel sdata4
	.uleb128 Lttbase0-Lttbaseref0
Lttbaseref0:                        ## ---- LSDA Call Site header section -----
	.byte	1                       ## Call site Encoding = uleb128
	.uleb128 Lcst_end0-Lcst_begin0
Lcst_begin0:                        ## ---- LSDA Call Site section -----
	.uleb128 Ltmp0-Lfunc_begin0     ## >> Call Site 1 <<
	.uleb128 Ltmp1-Ltmp0            ##   Call between Ltmp0 and Ltmp1
	.uleb128 Ltmp2-Lfunc_begin0     ##     jumps to Ltmp2
	.byte	1                       ##   On action: 1
	.uleb128 Ltmp1-Lfunc_begin0     ## >> Call Site 2 <<
	.uleb128 Ltmp3-Ltmp1            ##   Call between Ltmp1 and Ltmp3
	.byte	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp3-Lfunc_begin0     ## >> Call Site 3 <<
	.uleb128 Ltmp4-Ltmp3            ##   Call between Ltmp3 and Ltmp4
	.uleb128 Ltmp5-Lfunc_begin0     ##     jumps to Ltmp5
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp4-Lfunc_begin0     ## >> Call Site 4 <<
	.uleb128 Ltmp8-Ltmp4            ##   Call between Ltmp4 and Ltmp8
	.byte	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp8-Lfunc_begin0     ## >> Call Site 5 <<
	.uleb128 Ltmp9-Ltmp8            ##   Call between Ltmp8 and Ltmp9
	.uleb128 Ltmp10-Lfunc_begin0    ##     jumps to Ltmp10
	.byte	1                       ##   On action: 1
	.uleb128 Ltmp6-Lfunc_begin0     ## >> Call Site 6 <<
	.uleb128 Ltmp7-Ltmp6            ##   Call between Ltmp6 and Ltmp7
	.uleb128 Ltmp16-Lfunc_begin0    ##     jumps to Ltmp16
	.byte	3                       ##   On action: 2
	.uleb128 Ltmp7-Lfunc_begin0     ## >> Call Site 7 <<
	.uleb128 Ltmp11-Ltmp7           ##   Call between Ltmp7 and Ltmp11
	.byte	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp11-Lfunc_begin0    ## >> Call Site 8 <<
	.uleb128 Ltmp12-Ltmp11          ##   Call between Ltmp11 and Ltmp12
	.uleb128 Ltmp13-Lfunc_begin0    ##     jumps to Ltmp13
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp12-Lfunc_begin0    ## >> Call Site 9 <<
	.uleb128 Ltmp14-Ltmp12          ##   Call between Ltmp12 and Ltmp14
	.byte	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp14-Lfunc_begin0    ## >> Call Site 10 <<
	.uleb128 Ltmp15-Ltmp14          ##   Call between Ltmp14 and Ltmp15
	.uleb128 Ltmp16-Lfunc_begin0    ##     jumps to Ltmp16
	.byte	3                       ##   On action: 2
	.uleb128 Ltmp15-Lfunc_begin0    ## >> Call Site 11 <<
	.uleb128 Lfunc_end0-Ltmp15      ##   Call between Ltmp15 and Lfunc_end0
	.byte	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
Lcst_end0:
	.byte	1                       ## >> Action Record 1 <<
                                        ##   Catch TypeInfo 1
	.byte	0                       ##   No further actions
	.byte	2                       ## >> Action Record 2 <<
                                        ##   Catch TypeInfo 2
	.byte	0                       ##   No further actions
	.p2align	2
                                        ## >> Catch TypeInfos <<
	.long	0                       ## TypeInfo 2
	.long	__ZTI14Fake_Exception@GOTPCREL+4 ## TypeInfo 1
Lttbase0:
	.p2align	2
                                        ## -- End function
	.section	__TEXT,__text,regular,pure_instructions
	.private_extern	___clang_call_terminate ## -- Begin function __clang_call_terminate
	.globl	___clang_call_terminate
	.weak_definition	___clang_call_terminate
	.p2align	4, 0x90
___clang_call_terminate:                ## @__clang_call_terminate
## %bb.0:
	pushq	%rax
	callq	___cxa_begin_catch
	movq	%rax, (%rsp)            ## 8-byte Spill
	callq	__ZSt9terminatev
                                        ## -- End function
	.globl	__Z7catchitv            ## -- Begin function _Z7catchitv
	.p2align	4, 0x90
__Z7catchitv:                           ## @_Z7catchitv
Lfunc_begin1:
	.cfi_startproc
	.cfi_personality 155, ___gxx_personality_v0
	.cfi_lsda 16, Lexception1
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
Ltmp17:
	callq	__Z18try_but_dont_catchv
Ltmp18:
	jmp	LBB3_1
LBB3_1:
	jmp	LBB3_6
LBB3_2:
Ltmp19:
                                        ## kill: def $edx killed $edx killed $rdx
	movq	%rax, -8(%rbp)
	movl	%edx, -12(%rbp)
## %bb.3:
	movl	-12(%rbp), %eax
	movl	$2, %ecx
	cmpl	%ecx, %eax
	movl	%eax, -36(%rbp)         ## 4-byte Spill
	jne	LBB3_7
## %bb.4:
	movq	-8(%rbp), %rdi
	callq	___cxa_begin_catch
	movq	%rax, -32(%rbp)
Ltmp25:
	leaq	L_.str.5(%rip), %rdi
	xorl	%ecx, %ecx
                                        ## kill: def $cl killed $cl killed $ecx
	movb	%cl, %al
	callq	_printf
Ltmp26:
	jmp	LBB3_5
LBB3_5:
	callq	___cxa_end_catch
LBB3_6:
	leaq	L_.str.6(%rip), %rdi
	movb	$0, %al
	callq	_printf
	addq	$48, %rsp
	popq	%rbp
	retq
LBB3_7:
	movl	$1, %eax
	movl	-36(%rbp), %ecx         ## 4-byte Reload
	cmpl	%eax, %ecx
	jne	LBB3_14
## %bb.8:
	movq	-8(%rbp), %rdi
	callq	___cxa_begin_catch
	movq	%rax, -24(%rbp)
Ltmp20:
	leaq	L_.str.4(%rip), %rdi
	xorl	%ecx, %ecx
                                        ## kill: def $cl killed $cl killed $ecx
	movb	%cl, %al
	callq	_printf
Ltmp21:
	jmp	LBB3_9
LBB3_9:
	callq	___cxa_end_catch
	jmp	LBB3_6
LBB3_10:
Ltmp22:
                                        ## kill: def $edx killed $edx killed $rdx
	movq	%rax, -8(%rbp)
	movl	%edx, -12(%rbp)
Ltmp23:
	callq	___cxa_end_catch
Ltmp24:
	jmp	LBB3_11
LBB3_11:
	jmp	LBB3_14
LBB3_12:
Ltmp27:
                                        ## kill: def $edx killed $edx killed $rdx
	movq	%rax, -8(%rbp)
	movl	%edx, -12(%rbp)
Ltmp28:
	callq	___cxa_end_catch
Ltmp29:
	jmp	LBB3_13
LBB3_13:
	jmp	LBB3_14
LBB3_14:
	movq	-8(%rbp), %rdi
	callq	__Unwind_Resume
	ud2
LBB3_15:
Ltmp30:
	movq	%rax, %rdi
	movq	%rdx, -48(%rbp)         ## 8-byte Spill
	callq	___clang_call_terminate
Lfunc_end1:
	.cfi_endproc
	.section	__TEXT,__gcc_except_tab
	.p2align	2
GCC_except_table3:
Lexception1:
	.byte	255                     ## @LPStart Encoding = omit
	.byte	155                     ## @TType Encoding = indirect pcrel sdata4
	.uleb128 Lttbase1-Lttbaseref1
Lttbaseref1:
	.byte	1                       ## Call site Encoding = uleb128
	.uleb128 Lcst_end1-Lcst_begin1
Lcst_begin1:
	.uleb128 Ltmp17-Lfunc_begin1    ## >> Call Site 1 <<
	.uleb128 Ltmp18-Ltmp17          ##   Call between Ltmp17 and Ltmp18
	.uleb128 Ltmp19-Lfunc_begin1    ##     jumps to Ltmp19
	.byte	3                       ##   On action: 2
	.uleb128 Ltmp18-Lfunc_begin1    ## >> Call Site 2 <<
	.uleb128 Ltmp25-Ltmp18          ##   Call between Ltmp18 and Ltmp25
	.byte	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp25-Lfunc_begin1    ## >> Call Site 3 <<
	.uleb128 Ltmp26-Ltmp25          ##   Call between Ltmp25 and Ltmp26
	.uleb128 Ltmp27-Lfunc_begin1    ##     jumps to Ltmp27
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp26-Lfunc_begin1    ## >> Call Site 4 <<
	.uleb128 Ltmp20-Ltmp26          ##   Call between Ltmp26 and Ltmp20
	.byte	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp20-Lfunc_begin1    ## >> Call Site 5 <<
	.uleb128 Ltmp21-Ltmp20          ##   Call between Ltmp20 and Ltmp21
	.uleb128 Ltmp22-Lfunc_begin1    ##     jumps to Ltmp22
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp21-Lfunc_begin1    ## >> Call Site 6 <<
	.uleb128 Ltmp23-Ltmp21          ##   Call between Ltmp21 and Ltmp23
	.byte	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
	.uleb128 Ltmp23-Lfunc_begin1    ## >> Call Site 7 <<
	.uleb128 Ltmp29-Ltmp23          ##   Call between Ltmp23 and Ltmp29
	.uleb128 Ltmp30-Lfunc_begin1    ##     jumps to Ltmp30
	.byte	5                       ##   On action: 3
	.uleb128 Ltmp29-Lfunc_begin1    ## >> Call Site 8 <<
	.uleb128 Lfunc_end1-Ltmp29      ##   Call between Ltmp29 and Lfunc_end1
	.byte	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
Lcst_end1:
	.byte	1                       ## >> Action Record 1 <<
                                        ##   Catch TypeInfo 1
	.byte	0                       ##   No further actions
	.byte	2                       ## >> Action Record 2 <<
                                        ##   Catch TypeInfo 2
	.byte	125                     ##   Continue to action 1
	.byte	3                       ## >> Action Record 3 <<
                                        ##   Catch TypeInfo 3
	.byte	0                       ##   No further actions
	.p2align	2
                                        ## >> Catch TypeInfos <<
	.long	0                       ## TypeInfo 3
	.long	__ZTI9Exception@GOTPCREL+4 ## TypeInfo 2
	.long	__ZTI14Fake_Exception@GOTPCREL+4 ## TypeInfo 1
Lttbase1:
	.p2align	2
                                        ## -- End function
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_seppuku                ## -- Begin function seppuku
	.p2align	4, 0x90
_seppuku:                               ## @seppuku
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	callq	__Z7catchitv
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__const
	.globl	__ZTS9Exception         ## @_ZTS9Exception
	.weak_definition	__ZTS9Exception
__ZTS9Exception:
	.asciz	"9Exception"

	.section	__DATA,__const
	.globl	__ZTI9Exception         ## @_ZTI9Exception
	.weak_definition	__ZTI9Exception
	.p2align	3
__ZTI9Exception:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTS9Exception

	.section	__TEXT,__const
	.globl	__ZTS14Fake_Exception   ## @_ZTS14Fake_Exception
	.weak_definition	__ZTS14Fake_Exception
__ZTS14Fake_Exception:
	.asciz	"14Fake_Exception"

	.section	__DATA,__const
	.globl	__ZTI14Fake_Exception   ## @_ZTI14Fake_Exception
	.weak_definition	__ZTI14Fake_Exception
	.p2align	3
__ZTI14Fake_Exception:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTS14Fake_Exception

	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"Running a try which will never throw.\n"

L_.str.1:                               ## @.str.1
	.asciz	"Exception caught... with the wrong catch!\n"

L_.str.2:                               ## @.str.2
	.asciz	"Caught a Fake_Exception... with the wrong catch!\n"

L_.str.3:                               ## @.str.3
	.asciz	"try_but_dont_catch handled the exception\n"

L_.str.4:                               ## @.str.4
	.asciz	"Caught an Fake_Exception with the right catch!\n"

L_.str.5:                               ## @.str.5
	.asciz	"Caught an Exception with the right catch!\n"

L_.str.6:                               ## @.str.6
	.asciz	"catchit handled the exception\n"

.subsections_via_symbols
