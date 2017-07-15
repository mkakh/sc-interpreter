#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CODE_SIZE 2000
#define OP_SIZE 2000
#define STACK_SIZE 10000

/* 関数プロトタイプ宣言 */
void read(const char *, int *, int *);
int bop(int, int, int);
int uop(int, int);

enum opcode {
	LDC, LLD, LGD, LLA, LGA, STL, STG, STO, IND, UJP,
	FJP, ENT, MST, CUP, RET, BOP, UOP, HLT, ISP, DSP, IBS, DUP, STF, INP, OUT, OCH
};
enum binaryop {
	ADD, SUB, MUL, DIV, MOD, AND, OR, GT, GE, LT, LE, EQ, NOT_EQ
};
enum unaryop {
	NOT, UMINUS
};

const char *opcode_str[] = {"LDC", "LLD", "LGD", "LLA", "LGA", "STL", "STG", "STO", "IND",
			    "UJP", "FJP", "ENT", "MST", "CUP", "RET", "BOP", "UOP", "HLT", "ISP", "DSP", "IBS",
			    "DUP", "STF", "INP", "OUT", "OCH"};
const char *binaryop_str[] = {"ADD", "SUB", "MUL", "DIV", "MOD", "AND", "OR", "GT", "GE", "LT", "LE",
                            "EQ", "NOT_EQ"};
const char *binaryop_str2[] = {"+", "-", "*", "/", "%", "&&", "||", ">", ">=", "<", "<=", "==", "!="};
const char *unaryop_str[] = {"NOT", "UMINUS"};
const char *unaryop_str2[] = {"!", "-"};
const char *no_operand_str[] = {"IN", "OUT", "OCH", "HLT", "STO", "MST", "DUP", "STF"};


int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Usage: %s <file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	int code[CODE_SIZE]; 
	int operand[OP_SIZE];
	int stack[STACK_SIZE];
	int sp, base, pc, op, a;

        /* ファイル読み込み */
	read(argv[1], code, operand);

	sp = base = pc = 1;
	op = code[1];
	a = operand[1];

	while (op != HLT) {
                pc = pc+1;
		/* 実行 */
		switch (op) {
		        case LDC: sp++; stack[sp] = a; break;
		        case LLD: sp++; stack[sp] = stack[base+a]; break;
		        case LGD: sp++; stack[sp] = stack[a]; break;
		        case LLA: sp++; stack[sp] = base+a; break;
		        case LGA: sp++; stack[sp] = a; break;
		        case STL: stack[base+a] = stack[sp]; sp--; break;
		        case STG: stack[a] = stack[sp]; sp--; break;
		        case STO: stack[stack[sp-1]]=stack[sp]; sp = sp-2; break;
		        case IND: stack[sp] = stack[stack[sp]+a]; break;
		        case UJP: pc = a; break;
		        case FJP: if(stack[sp] == 0) pc=a; sp--; break;
		        case ENT: sp = base + a; break;
		        case MST: stack[sp+1] = base; sp = sp + 2; break;
		        case CUP: stack[sp-a] = pc; base = sp-a-2; pc = stack[base]; break;
		        case RET: sp=base-a; pc=stack[base+2]; base=stack[base+1];break;
                        case BOP: sp--; stack[sp] = bop(stack[sp], stack[sp+1], a); break;
                        case UOP: stack[sp] = uop(stack[sp], a); break;
		        case HLT: break;
		        case ISP: sp = sp+a; break;
		        case DSP: sp = sp-a; break;
		        case IBS: base = base+a; break;
		        case DUP: sp++; stack[sp] = stack[sp-1]; break;
		        case STF: stack[stack[sp-1]]=stack[sp]; stack[sp-1] = stack[sp]; sp--; break;
		        case INP: sp++; printf("> "); scanf("%d", &stack[sp]); break;
                        case OUT: printf("%d\n", stack[sp]); break;
			case OCH: putchar(stack[sp]); break;
		        default: printf("%d: UNKNOWN OPCODE ERRROR\n", pc); exit(EXIT_FAILURE);
		}
		op = code[pc];
		a = operand[pc];
	}

	return EXIT_SUCCESS;
}

int uop(int st, int op)
{
        int ret;
        switch(op) {
	        case NOT:     ret = !st; break;
	        case UMINUS:  ret = -st; break;
	        default:      puts("UOP ERROR"); exit(EXIT_FAILURE);
        }
        return ret;
}

int bop(int st1, int st2, int op)
{
        int ret;
        switch (op) {
	        case ADD:     ret = st1 + st2;  break;
	        case SUB:     ret = st1 - st2;  break;
	        case MUL:     ret = st1 * st2;  break;
	        case DIV:     ret = st1 / st2;  break;
	        case MOD:     ret = st1 % st2;  break;
	        case AND:     ret = st1 && st2; break;
	        case OR:      ret = st1 || st2; break;
	        case GT:      ret = st1 > st2;  break;
	        case GE:      ret = st1 >= st2; break;
	        case LT:      ret = st1 < st2;  break;
	        case LE:      ret = st1 <= st2; break;
	        case EQ:      ret = st1 == st2; break;
	        case NOT_EQ:  ret = st1 != st2; break;
	        default:      puts("BOP ERROR"); exit(EXIT_FAILURE);
        }
        return ret;
}

void read(const char *file, int code[], int operand[])
{
	FILE *fp;
	int i;
	char buf[100];
	char *p;
	int pc = 1;
	if ((fp = fopen(file, "r")) == NULL) {
		puts("FILE OPEN ERROR");
		exit(EXIT_FAILURE);
	}
	while ((fgets(buf, sizeof(buf), fp) != NULL)) {
		p = buf;
		/* オペコード読み取り */
		while (*p == ' ') { p++; }
		if ('\n' == *p || '\0' == *p) continue;
		if ('A' > *p && *p > 'Z') break;
		for (i = LDC; i <= OCH; i++) {
			if (!strncmp(p, opcode_str[i], strlen(opcode_str[i]))) {
				code[pc] = i;
				break;
			}
			if (i == OCH) {
				puts("UNKNOWN OPCODE ERROR");
				exit(EXIT_FAILURE);
			}
		}
                for (i = 0; i < 7; i++) {
                        if (!strncmp(p, no_operand_str[i], strlen(no_operand_str[i]))) {
                                goto OPERAND_READING_END;
                        }
                }

		/* オペランド読み取り */
		p += 3;
		while (*p == ' ') { p++; }
		if ('0' <= *p && *p <= '9') {
			operand[pc] = atoi(p);
			goto OPERAND_READING_END;
		}
		for (i = ADD; i <= NOT_EQ; i++) {
			if (!strncmp(p, binaryop_str[i], strlen(binaryop_str[i]))) {
				operand[pc] = i;
				goto OPERAND_READING_END;
			}
			if (!strncmp(p, binaryop_str2[i], strlen(binaryop_str2[i]))) {
				operand[pc] = i;
				goto OPERAND_READING_END;
			}
		}
		for (i = NOT; i <= UMINUS; i++) {
			if (!strncmp(p, unaryop_str[i], strlen(unaryop_str[i]))) {
				operand[pc] = i;
				goto OPERAND_READING_END;
			}
			if (!strncmp(p, unaryop_str2[i], strlen(unaryop_str2[i]))) {
				operand[pc] = i;
				goto OPERAND_READING_END;
			}
		}
                printf("%d: UNKNOWN OPERAND\n", pc);
	OPERAND_READING_END: pc++;
	}
	fclose(fp);
}
