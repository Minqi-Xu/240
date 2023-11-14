#include <iostream>
#include <vector>
#include<string>
using namespace std;


// Your code goes here

string dictionary[4096];

void initializing(int l, int r, string s) {
    for(int i = l; i < r; ++i)
        dictionary[i] += s;
}

void adding(int idx, int mod) {
    switch(mod) {
        case 0:
            dictionary[idx] += "a";
            break;
        case 1:
            dictionary[idx] += "b";
            break;
        case 2:
            dictionary[idx] += "c";
            break;
        case 3:
            dictionary[idx] += "d";
            break;
        case 4:
            dictionary[idx] += "e";
            break;
        case 5:
            dictionary[idx] += "f";
            break;
        case 6:
            dictionary[idx] += "g";
            break;
        case 7:
            dictionary[idx] += "h";
            break;
        case 8:
            dictionary[idx] += "i";
            break;
        case 9:
            dictionary[idx] += "j";
            break;
        case 10:
            dictionary[idx] += "k";
            break;
        case 11:
            dictionary[idx] += "l";
            break;
        case 12:
            dictionary[idx] += "m";
            break;
        case 13:
            dictionary[idx] += "n";
            break;
        case 14:
            dictionary[idx] += "o";
            break;
        case 15:
            dictionary[idx] += "p";
            break;
        case 16:
            dictionary[idx] += "q";
            break;
        case 17:
            dictionary[idx] += "r";
            break;
        case 18:
            dictionary[idx] += "s";
            break;
        case 19:
            dictionary[idx] += "t";
            break;
        case 20:
            dictionary[idx] += "u";
            break;
        case 21:
            dictionary[idx] += "v";
            break;
        case 22:
            dictionary[idx] += "w";
            break;
        case 23:
            dictionary[idx] += "x";
            break;
        case 24:
            dictionary[idx] += "y";
            break;
        case 25:
            dictionary[idx] += "z";
            break;
    }
}

int convert(string s, int idx) {
    int sum = 0;
    for(int i = 0; i < 12; i++) {
        sum = sum * 2;
        sum += s[idx] - 48;
        idx++;
    }
    return sum;
}

void LZW_decode_new(const string& coded_s, string& s)
{
    // initializing dictionary
    for(int i = 0; i < 4096; ++i) {
        dictionary[i] = "";
    }
    initializing(26,52,"a");
    initializing(52,78,"b");
    initializing(78,104,"c");
    initializing(104,130,"d");
    initializing(130,156,"e");
    initializing(156,182,"f");
    initializing(182,208,"g");
    initializing(208,234,"h");
    initializing(234,260,"i");
    initializing(260,286,"j");
    initializing(286,312,"k");
    initializing(312,338,"l");
    initializing(338,364,"m");
    initializing(364,390,"n");
    initializing(390,416,"o");
    initializing(416,442,"p");
    initializing(442,468,"q");
    initializing(468,494,"r");
    initializing(494,520,"s");
    initializing(520,546,"t");
    initializing(546,572,"u");
    initializing(572,598,"v");
    initializing(598,624,"w");
    initializing(624,650,"x");
    initializing(650,676,"y");
    initializing(676,702,"z");
    for(int i = 0; i < 702; ++i) {
        adding(i, i%26);
    }
    int idx = 702;
    int i = 0;
    int code = convert(coded_s, i);
    i += 12;
    string str = dictionary[code];
    s += str;
    while(i + 11 < coded_s.size()) {
        string s_pre = str;
        code = convert(coded_s, i);
        i += 12;
        if(code < idx)
            str = dictionary[code];
        else if(code == idx)
            str = s_pre + s_pre[0];
        else if(code == idx + 1)
            str = s_pre + s_pre[0] + s_pre[1];
        else {
            cerr << "ERROR!" << endl;
            return;
        }
        s += str;
        dictionary[idx] = s_pre + str[0];
        idx++;
        dictionary[idx] = s_pre + str[0] + str[1];
        idx++;
    }
}


int main()
{
    string s = "";
    string coded_s;
    getline(cin, coded_s);
    LZW_decode_new(coded_s, s);
    cout << s;
}

