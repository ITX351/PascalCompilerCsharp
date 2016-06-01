#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include "constants.h"
using namespace std;

//Love lemon_TsyD
//Miss lydrainbowcat
//By ITX351

class Scanner
{
private:
    typedef pair < int, string > IS;

    FILE *fp_r, *fp_w;
    int bufferTop;
    char buffer[BUFFER_SIZE];

    char thisWord[BUFFER_SIZE];

    map < string, TokenType > table;

    char nextChar()
    {
        if (bufferTop > 0)
        {
            return buffer[--bufferTop];
        }
        return fgetc(fp_r);
    };

    void backForward(char now)
    {
        buffer[bufferTop++] = now;
    }

    void dealScan(TokenType tokenType, const char *word)
    {
        fprintf(fp_w, "(%d, %s)\n", (int)tokenType, word);
        result.push_back(IS((int)tokenType, string(word)));
    }

    bool isSpace(char now)
    {
        return now == ' ' || now == '\n' || now == '\t';
    }

    bool isAlpha(char now)
    {
        return (now >= 'A' && now <= 'Z') || (now >= 'a' && now <= 'z') || now == '$' || now == '_';
    }

    bool isDigit(char now)
    {
        return now >= '0' && now <= '9';
    }
public:
    map < string, int > signTable; int signs;
    vector < IS > result;

    Scanner() { }
    void init()
    {
        result.clear();

        fp_r = fopen("source.pas", "r");
        fp_w = fopen("words.txt", "w");

        bufferTop = 0;

        for (int i = 0; i < KEYWORD_NUM; i++)
        {
            string str = string(keywordList[i]);
            table[str] = (TokenType)i;
        }
    }

    void close()
    {
        fclose(fp_r);
        fclose(fp_w);
    }

    void scan()
    {
        char ch; int loc; char quote;
        signs = 0;
        while ((ch = nextChar()) != EOF)
        {
            if (isSpace(ch))
            {
                while (isSpace(ch))
                    ch = nextChar();
                backForward(ch);
            }
            else if (isAlpha(ch))
            {
                loc = 0;
                do
                {
                    thisWord[loc++] = ch;
                    ch = nextChar();
                }while (isAlpha(ch) || isDigit(ch));
                thisWord[loc] = '\0';
                backForward(ch);

                if (table.find(string(thisWord)) != table.end())
                    dealScan(table[string(thisWord)], thisWord);
                else
                {
                    dealScan(T_IDN, thisWord);
                    if (signTable.find(thisWord) == signTable.end())
                        signTable[thisWord] = ++signs;
//                    signTable[thisWord] = signTable[thisWord] + 1;
                }
            }
            else if (isDigit(ch))
            {
                bool real = false;

                loc = 0;
                do
                {
                    if (ch == '.')
                    {
                        if (real)
                        {
                            backForward('.'); // back two '.'
                            loc--;
                            real = false;
                            break;
                        }
                        real = true;
                    }

                    thisWord[loc++] = ch;
                    ch = nextChar();
                } while (isDigit(ch) || ch == '.');
                thisWord[loc] = '\0';
                backForward(ch);

                if (real)
                    dealScan(T_INT, thisWord);
                else
                    dealScan(T_REAL, thisWord);
            }
            else
            {
                switch (ch)
                {
                case ':':
                    if ((ch = nextChar()) == '=')
                        dealScan(T_ASS, ":=");
                    else
                    {
                        backForward(ch);
                        dealScan(T_COLON, ":");
                    }
                    break;
                case '*':
                    switch (ch = nextChar())
                    {
                    case '*':
                        dealScan(T_OTHERS, "**");
                        break;
                    case '=':
                        dealScan(T_OTHERS, "*=");
                        break;
                    default:
                        backForward(ch);
                        dealScan(T_MUL, "*");
                        break;
                    }
                    break;
                case '+':
                    if ((ch = nextChar()) == '=')
                        dealScan(T_OTHERS, "+=");
                    else
                    {
                        backForward(ch);
                        dealScan(T_ADD, "+");
                    }
                    break;
                case '-':
                    if ((ch = nextChar()) == '=')
                        dealScan(T_OTHERS, "-=");
                    else
                    {
                        backForward(ch);
                        dealScan(T_SUB, "-");
                    }
                    break;
                case '(':
                    dealScan(T_LPAR, "(");
                    break;
                case ')':
                    dealScan(T_RPAR, ")");
                    break;
                case '.':
                    if ((ch = nextChar()) == '.')
                        dealScan(T_DOUBLEPERIOD, "..");
                    else
                    {
                        backForward(ch);
                        dealScan(T_OTHERS, ".");
                    }
                    break;
                case ',':
                    dealScan(T_COMMA, ",");
                    break;
                case '=':
                    dealScan(T_EQL, "=");
                    break;
                case ';':
                    dealScan(T_SEMICL, ";");
                    break;
                case '[':
                    dealScan(T_LBRKPAR, "[");
                    break;
                case ']':
                    dealScan(T_RBRKPAR, "]");
                    break;
                case '<':
                    ch = nextChar();
                    if (ch == '>')
                        dealScan(T_NEQ, "<>");
                    else if (ch == '=')
                        dealScan(T_LTE, "<=");
                    else
                    {
                        backForward(ch);
                        dealScan(T_LT, "<");
                    }
                    break;
                case '>':
                    if ((ch = nextChar()) == '=')
                        dealScan(T_GT, ">=");
                    else
                    {
                        backForward(ch);
                        dealScan(T_GTE, ">");
                    }
                    break;
                case '/':
                    if ((ch = nextChar()) == '/')
                        while (ch != '\n' && ch != EOF)
                            ch = nextChar();
                    else
                    {
                        backForward(ch);
                        dealScan(T_SLASH, "/");
                    }
                    break;
                case '\"':
                case '\'':
                    quote = ch;

                    loc = 0;
                    do
                    {
                        thisWord[loc++] = ch;
                        ch = nextChar();
                    } while (ch != quote && ch != EOF);
                    thisWord[loc++] = quote;
                    thisWord[loc] = '\0';

                    dealScan(T_OTHERS, thisWord);
                    break;
                case '{':
                    while (ch != '}' && ch != EOF)
                        ch = nextChar();
                    break;
                default:
                    printf("%c\n", ch);
                    break;
                }
            } // if - else
        } // while

        dealScan(T_EOF, "#");

        fprintf(fp_w, "\n");
        for (map<string, int >::iterator it = signTable.begin(); it != signTable.end(); it++)
            fprintf(fp_w, "%s %d\n", it->first.c_str(), it->second);
    }
};
