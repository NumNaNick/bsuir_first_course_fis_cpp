//14. Дана строка, состоящая из слов, разделенных пробелами. Вывести на экран порядковый 
//номер слова с максимальной длиной и номер позиции строки, с которой оно начинается.
//s - строка
//ml - максимальная длина
//wn - номер слова
//sp - стартовая позиция
//cw - текущее слово
//cs - текущий старт (откуда начинается отсчет слова)
//cl - текущая длина
#include <iostream>
#include <string>

using namespace std;

int main() {
    string s;
    cout << "Write your phrase: ";
    getline(cin, s);

    int ml = 0, wn = 0, sp = 0, cw = 0;
    int i = 0, n = s.length();

    while (i < n) {
        while (i < n && s[i] == ' ') i++;
        if (i >= n) break;

        cw++;
        int cs = i, cl = 0;

        while (i < n && s[i] != ' ') {
            cl++;
            i++;
        }

        if (cl > ml) {
            ml = cl;
            wn = cw;
            sp = cs;
        }
    }

    if (wn > 0) {
        cout << "Word position: " << wn << endl;
        cout << "Beginning position: " << sp << endl;
    }
    else {
        cout << "Phrase is null" << endl;
    }

    return 0;
}