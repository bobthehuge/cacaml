#include <common.h>
#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

#include "cml_module_node.h"
#include "cml_lexer.h"
#include "cml_parser.h"
#include "cml_token.h"
// #include "common.h"

#define BTH_CSTR_IMPLEMENTATION
#include "../utils/bth_log.h"
#define BTH_SALLOC_IMPLEMENTATION
#include "../utils/bth_salloc.h"
#define BTH_DYNARRAY_IMPLEMENTATION
#include "../utils/bth_dynarray.h"

#define TOK_MASK 0x01
#define AST_MASK 0x02
#define SSA_MASK 0x04
#define ASM_MASK 0x08

void print_usage(char **argv)
{
    fprintf(stderr, "%s [OPTION] {file.cml, ...}\n", argv[0]);
    fprintf(stderr, "Debug operations (except help) are directed to {output}");
    fprintf(stderr, " file which defaults to stdout\n");
    fprintf(stderr, "\t%-11s prints this help message\n", "-h");
    fprintf(stderr, "\t%-11s file output file\n", "-o");
    fprintf(stderr, "\t%-11s output produced Tokens\n", "-T");
    fprintf(stderr, "\t%-11s output produced AST\n", "-P");
}

int main(int argc, char **argv)
{
    int c;
    FILE *outf = stdout;
    char *f;
    char flags = 0;

    while ((c = getopt(argc, argv, "hTPo:")) != -1)
    {
        switch (c)
        {
        // output
        case 'o':
			if (strcmp(optarg, "-") != 0) 
			{
				outf = fopen(optarg, "w");
				if (!outf)
				{
				    print_usage(argv);
					ERRX(1, "Cannot open '%s'\n", optarg);
				}
			}
            break;

        // lex only
        case 'T':
            flags |= TOK_MASK;
            break;
        // parse only
        case 'P':
            flags |= AST_MASK;
            break;
        case 'h':
        /* FALLTHROUGH */
        default:
            print_usage(argv);
            exit(1);
            break;
        }
    }

    struct cml_lexer lex;
    struct cml_parser pa;
    struct cml_module_node *mods = smalloc(0);
    size_t modc = 0;

	do 
	{
		f = argv[optind];
		if (!f || strcmp(f, "-") == 0)
	    {
	        print_usage(argv);
		    ERRX(1, "No input file provided");
		}

        modc++;
        mods = srealloc(mods, modc * sizeof(struct cml_module_node));

		cml_lexer_from_file(&lex, f);
		pa.tokens = cml_lexer_lexall(&lex);

        if (flags & TOK_MASK)
        {
            for (uint32_t i = 0; i < pa.tokens.len; i++)
            {
                struct cml_token tok;
                bth_dynarray_get(&pa.tokens, i, &tok);
                fprintf(
                    outf,
                    "{ .kind: %s, .value: \"%s\" }\n",
                    cml_tkind2str(tok.kind),
                    tok.value
                );
            }
            continue;
        }
		
		cml_parser_init(&pa);
		cml_parse_module(&pa, &mods[modc-1]);

		mods[modc-1].name = f;

		if (flags & AST_MASK)
		{
		    cml_print_module(&mods[modc-1], outf);
		    continue;
		}

		cml_emit_module(&mods[modc-1], outf);

		if (flags & SSA_MASK)
		    continue;

	} while (++optind < argc);

	if (!(flags & (TOK_MASK | AST_MASK)))
	{
        fprintf(outf, "export function w $main() {\n");
        fprintf(outf, "@start\n");

        for (size_t i = 0; i < modc; i++)
        {
            fprintf(outf, "    call $%s\n", mods[i].name);
            cml_free_module(mods + i);
        }

        fprintf(outf, "    ret 0\n}\n");
        fclose(outf);
    }

    cml_lexer_destroy(&lex);
    cml_free_parser(&pa);
    free(mods);

    return 0;
}
