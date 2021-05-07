	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 15	sdk_version 10, 15, 6
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
	movl	$1, %edi
	callq	___cxa_allocate_exception
	movq	__ZTI9Exception@GOTPCREL(%rip), %rsi
	xorl	%ecx, %ecx
	movl	%ecx, %edx
	movq	%rax, %rdi
	callq	___cxa_throw
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

.subsections_via_symbols
