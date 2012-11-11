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
 * Copyright 2012 Patrick Haas (@patmanteau)
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

// Wieviele Zeichen darf die Eingabezeile maximal enthalten?
// size_t dient der Portabilität (und ist ein vorzeichenloser
// Ganzzahltyp)
const size_t USER_INPUT_BUFFER_SIZE = 8192;

int main(const int argc, const char **argv) {

    // Eingabepuffer als "String" mit Maximallänge
    char *user_input = (char *)calloc(USER_INPUT_BUFFER_SIZE, sizeof(char));
    
    // Eingabeschleife als REPL(read-eval-print loop) -> Eingabe, Verarbeitung, Ausgabe
    do {
        // Eingabepuffer löschen
        memset(user_input, 0, USER_INPUT_BUFFER_SIZE);
        
        //////////////////////
        // read
        //////////////////////
        
        printf("?: ");

        // Bei end-of-file oder Lesefehler gibt fgets NULL zurück
        // und das Programm soll beendet werden (analog zur Shell)
        //
        // break ist eigentlich nicht schön (=nicht im Struktogramm
        // abbildbar ;) ), dient aber hier der Klarheit, da einige
        // if entfallen können
        if (!fgets(user_input, USER_INPUT_BUFFER_SIZE, stdin)) break;
        strim(user_input);

        //////////////////////
        // eval
        //////////////////////
        
        // Abbruch durch User?
        if (strcmp(user_input, "\\quit") == 0) break;

        // 1. Parsen
        parse_context *ctx = start_parse(user_input);
        parse(ctx);

        // 2. Baum traversieren
        if (ctx->ast_root != NULL) {
            printf("-> ");
            print_expr_ast(ctx->ast_root);
            double result = calc_expr_ast(ctx->ast_root);
            //////////////////////
            // print
            //////////////////////
            printf(" = %g\n", result);
        }
        // Aufräumen
        end_parse(ctx);
    } while (1); // loop....

    printf("Quitting.\n");

    // Speicher freigeben
    free(user_input);

    return 0;
}
