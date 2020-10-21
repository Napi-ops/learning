#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**************************************
 * Function name: match
 * Intention: from string str_t find which part can match string str_p
 * Parameter: Array "str_t","str_p" - two string which need to match
 *            Int "t_len","p_len" - the length of str_t and str_p
 *            Array "match_str" - the result of match string
 * Return Value: Int "num" - the total number of match string
 * ************************************/
int match(char str_t[550], char str_p[550], int t_len, int p_len, int match_str[550][2])
{
    int i = 0, j = 0, k = 0; //Loop variable
    int mark = 0;   //mark is the number of question mark
    int tag = 1;    //tag decide if two string can match 
    int num = 0;    //the total number of match string

    //get the value of mark
    for (mark = 0; str_p[mark] == '?' && mark != p_len; mark++);

    //start matching
    //if the string is all "?"
    if (mark == p_len)
    {
        for (i = p_len - 1; i != t_len; i++, j++)
        {
            match_str[j][0] = j;
            match_str[j][1] = p_len;
            num++;
        }
    }
    //else if the string is not all "?"
    else
    {
        for (i = mark; i != t_len; i++)
        {
            tag = 1;
            if ((str_t[i] == str_p[mark]) || ((str_t[i] - 'a') == (str_p[mark] - 'A')) || ((str_t[i] - 'A') == (str_p[mark] - 'a')))
            {
                for (j = i + 1, k = mark + 1; j != i + p_len - mark; j++, k++)
                {
                    if (str_p[k] == '?')
                        continue;
                    else
                    {
                        if ((str_t[j] == str_p[k]) || ((str_t[j] - 'a') == (str_p[k] - 'A')) || ((str_t[j] - 'A') == (str_p[k] - 'a')))
                        {
                            if ((j + 1 == t_len) && (k + 1 != p_len))
                            {
                                tag = 0;
                                break;
                            }
                            else
                                continue;
                        }
                        else
                        {
                            tag = 0;
                            break;
                        }
                    }
                }
                if (tag)
                {
                    match_str[num][0] = i - mark;
                    match_str[num][1] = p_len;
                    num++;
                }
            }
        }
    }
    return num;
}

int main(void)
{
    int i = 0, j = 0, k = 0;
    char t[550] = { 0 };    //Array "t" is the string which is longer 
    char p[550] = { 0 };    //Array "P" is the string which is shorter
    char p_cut[550] = { 0 };    //Array "p_cut" is a part of string p, and it doesn't have hashtag
    char p_cut_2[550] = { 0 };  //Array "p_cut_2" if also a part of string p, and it doesn't have hashtag
    int match_str[550][2] = { {0},{0} };    //Array "match_str" contains the first subscript of the match string and the string length
    int match_str_2[550][2] = { {0},{0} };   //Array "match_str_2" also contains the first subscript of the match string and the string length
    int t_len = 0;  //t_len is the length of string t
    int p_len = 0;  //p_len is the length of string p
    int match_number = 0;   //match_number is the number of match string
    int match_number_2 = 0; //match_number_2 is also the number of match string
    int hashtag_num = -1;   //hashtag_num is the subscript of hashtag

    //get string t and p
    for (i = 0; (t[i] = getchar()) != '\n'; i++, t_len++);
    for (i = 0;; i++, p_len++)
    {
        p[i] = getchar();
        if (p[i] == EOF || p[i] == '\n')
            break;
        if (p[i] == '#')
            hashtag_num = i;
    }

    //start matching
    //if the string doesn't have a hashtag
    if (hashtag_num == -1)
    {
        match_number = match(t, p, t_len, p_len, match_str);
        for (i = 0; i != match_number; i++)
        {
            printf("%d ", match_str[i][0]);
            for (j = match_str[i][0]; j != match_str[i][1] + match_str[i][0]; j++)
                putchar(t[j]);
            putchar('\n');
        }
    }
    //else if string has a hashtag
    else
    {
        //if hashtag is on the first subscript
        if (p[0] == '#')
        {
            for (i = 0; i != p_len - 1; i++)
                p_cut[i] = p[i + 1];
            match_number = match(t, p_cut, t_len, p_len - 1, match_str);
            for (i = 0; i != match_str[match_number - 1][0] + 1; i++)
            {
                for (j = 0; j != match_number; j++)
                {
                    if (i > match_str[j][0])
                        continue;
                    else
                    {
                        printf("%d ", i);
                        for (k = i; k != match_str[j][1] + match_str[j][0]; k++)
                            putchar(t[k]);
                        putchar('\n');
                    }
                }
            }
        }
        //if hashtag is on the last subscript
        else if (p[p_len - 1] == '#')
        {
            for (i = 0; i != p_len - 1; i++)
                p_cut[i] = p[i];
            match_number = match(t, p_cut, t_len, p_len - 1, match_str);
            for (i = 0; i != match_number; i++)
            {
                for (k = match_str[i][1] + match_str[i][0]; k != t_len + 1; k++)
                {
                    printf("%d ", match_str[i][0]);
                    for (j = match_str[i][0]; j != k; j++)
                        putchar(t[j]);
                    putchar('\n');
                }
            }
        }
        //if hashtag is on the middle of a string
        else
        {
            for (i = 0; i != hashtag_num; i++)
                p_cut[i] = p[i];
            for (i = 0; i != p_len - hashtag_num - 1; i++)
                p_cut_2[i] = p[i + hashtag_num + 1];
            match_number = match(t, p_cut, t_len, hashtag_num, match_str);
            match_number_2 = match(t, p_cut_2, t_len, p_len - hashtag_num - 1, match_str_2);
            for (i = 0; i != match_number; i++)
            {
                for (j = 0; j != match_number_2; j++)
                {
                    if ((match_str_2[j][0] >= (match_str[i][0] + match_str[i][1])) && ((match_str_2[j][0] + match_str_2[j][1]) != (t_len + 1)))
                    {
                        printf("%d ", match_str[i][0]);
                        for (k = match_str[i][0]; k != match_str_2[j][0] + match_str_2[j][1]; k++)
                            putchar(t[k]);
                        putchar('\n');
                    }
                }
            }
        }
    }
}