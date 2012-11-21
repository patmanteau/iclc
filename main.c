/**
 * iclc - Interactive Command Line Calculator
 *
 * main.c
 *
 * iclc is a simple, interactive, command line calculator. It
 * employs and thereby demonstrates basic parsing technique. 
 *
 * Beware, it leaks like a strainer and probably has more bugs
 * than Starship Troopers.
 *
 *
 * Copyright 2012 Patrick Haas (@p7haas)
 *
 * Released under the MIT and GPL licenses.
 *
 * ------------------------------------------------
 *  author:  Patrick Haas
 *  url:     https://github.com/p7haas/iclc
 *  source:  https://github.com/p7haas/iclc
 */                                                   

#include "main.h"

#include "parser.h"
#include "eval.h"
#include "snippets.h"

#ifdef HAVE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

//////////////////////////////////////////////////////////////////////
//
// Helferfunktionen
//
//////////////////////////////////////////////////////////////////////


// Entfernt Whitespace (s. isspace) von Anfang und Ende
// von s.
char* strim(char * s) {
    char * p = s;
    size_t l = strlen(p);

    // Hinten Whitespace mit Nullen ersetzen
    while(isspace(p[l - 1])) p[--l] = 0;
    // Vorne den Pointer zum ersten Nicht-Whitespace bewegen
    while(* p && isspace(* p)) ++p, --l;

    // Teilstring an den Stringbeginn verschieben
    memmove(s, p, l + 1);

    return s;
}

// Gibt eine verkettete Fehlerliste auf stderr aus
void print_parse_errors(parse_context *ctx) {
    parse_error *error = ctx->error;
    while (error != NULL) {
        for (int i=0;i<error->position+2;i++) fprintf(stderr, " ");
        fprintf(stderr, "^ -- Error: %d: %s\n", error->position, error->message);
        error = error->_next;
    }
}

// Gibt eine verkettete Fehlerliste auf stderr aus
void print_eval_errors(eval_context *ctx) {
    eval_error *error = ctx->error;
    while (error != NULL) {
        fprintf(stderr, "Error: %s\n", error->message);
        error = error->__next;
    }
}

// Wieviele Zeichen darf die Eingabezeile maximal enthalten?
const size_t LINE_BUFFER_SIZE = 8192;

#ifdef HAVE_READLINE
//char *HISTORY_FILENAME = NULL;
const char *history_filename() {
    static char *name = NULL;

    if (!name) {
        char *homedir = getenv("HOME");
        const char *fname = "/.iclc_history";
        if (!homedir) {
            struct passwd *pw = getpwuid(getuid());
            homedir = pw->pw_dir;
        }
        name = calloc(strlen(homedir)+strlen(fname), sizeof(char));
        strcpy(name, homedir);
        strcpy(name+strlen(name), fname);
    }
    
    return name;
}
#endif // HAVE_READLINE


int repl() {
#ifdef HAVE_READLINE
    history_truncate_file(history_filename(), 1000);
    read_history(history_filename());
#endif // HAVE_READLINE


    // Eingabeschleife als REPL(read-eval-print loop) -> Eingabe, Verarbeitung, Ausgabe
    do {
        char *user_input = NULL;
        //////////////////////
        // read
        //////////////////////

#ifdef HAVE_READLINE
        user_input = readline("?: ");
        
        if (!user_input) break;
        strim(user_input);

        if (*user_input) add_history(user_input);
#else
        // Eingabepuffer als "String" mit Maximallänge
        user_input = calloc(LINE_BUFFER_SIZE, sizeof(char));
        printf("?: ");
        // Bei end-of-file oder Lesefehler gibt fgets NULL zurück
        // und das Programm soll beendet werden (analog zur Shell)
        //
        // break ist eigentlich nicht schön (=nicht im Struktogramm
        // abbildbar ;) ), dient aber hier der Klarheit, da einige
        // if entfallen können
        if (!fgets(user_input, LINE_BUFFER_SIZE, stdin)) break;
        strim(user_input);
#endif // HAVE_READLINE
        
        //////////////////////
        // eval
        //////////////////////
        
        // Abbruch durch User?
        if (strcmp(user_input, "\\quit") == 0) break;

        // 1. Parsen
        parse_context *p_ctx = start_parse(user_input);

        // 2. Baum traversieren
        if (parse(p_ctx) == 0) {
            eval_context *e_ctx = eval(p_ctx->ast_root);
            if (e_ctx == NULL) fprintf(stderr, "Fatal evaluation error.\n");
            if (e_ctx->success) {
                //////////////////////
                // print
                //////////////////////
                printf("-> %.50Lg\n", e_ctx->result);
            } else {
                print_eval_errors(e_ctx);
            }
            end_eval(e_ctx);
        } else {
            print_parse_errors(p_ctx);
        }
        // Aufräumen
        end_parse(p_ctx);
        free(user_input);
    } while (1); // loop....

#ifdef HAVE_READLINE
    write_history(history_filename());
#endif // HAVE_READLINE

    printf("End.\n");
    return 0;
}

int main(const int argc, const char **argv) {

    return repl();
}
