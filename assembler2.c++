#include<bits/stdc++.h>
using namespace std;
string temp;
vector<string> instruction;
bool format4, clear, compr, tixr;
map<string, string> optab;
map<string, string>::iterator it; 
string loc[100], name[100], ins[100], var[100], type[40];
int i = 2, add = 0, len = 0, b = 0;
// 把它補成4位
string add_zero(string str, int n) {
    while (str.length() < n) {
        str = "0" + str;
    }
    return str;
}
string turn_hex(long long int n){
    string ans, r;
    while (n != 0) {
        r += (n % 2 == 0 ? "0" : "1");
        n /= 2;
    }
    reverse(r.begin(), r.end());
    int len = r.length(), cnt = 0, temp = 0, i = 0;
    long long int intValue = std::stoll(r, 0, 2);
    // 使用 std::stringstream 将长整数转换为16进制字符串
    stringstream hexStream;
    hexStream << hex << intValue;
	string str = hexStream.str();
	for(auto &ch:str){
		if(islower(ch))
			ch -= ('a' - 'A');
	}
    return str;
}
int turn_decimal(const string loc){
    long long int ans = 0;
    for(int i = 0; i < loc.length(); i++){
        int tmp;
        if(isdigit(loc[i])){
            tmp = loc[i]-'0';
        }
        else{
            switch(loc[i]){
                case 'A': tmp = 10; break;
                case 'B': tmp = 11; break;
                case 'C': tmp = 12; break;
                case 'D': tmp = 13; break;
                case 'E': tmp = 14; break;
                case 'F': tmp = 15; break;
                default: break;
            }            
        }
        ans *= 16;
        ans += tmp;
    }
    return ans;
}
// 加上byte後回傳值
string now_pc(const string loc, int add){
    long long int decimal = turn_decimal(loc);
    string pc;
    if(add == 0) decimal += 3;
    else decimal += add;
    pc = turn_hex(decimal);
    return pc;
}
void now_instruction(int i){
    if(type[i] == "RESW" || type[i] == "RESB") add = stoi(instruction[2]);
    else if(type[i] == "BYTE"){
        if(instruction[2][0] == 'X') b = (instruction[2].length()-3) / 2;
        else len = instruction[2].length() - 3;
    } 
    else if(ins[i] == "CLEAR") clear = 1;
    else if(ins[i][0] == '+') format4 = 1;
    else if(ins[i] == "COMPR") compr = 1;
    else if(ins[i] == "TIXR") tixr = 1;
}
void now_add(int i){
    if(add > 48){
        loc[i] = now_pc(loc[i-1], add);
        add = 0;
    }
    else if(len){
        loc[i] = now_pc(loc[i-1], len);
        len = 0;
    }
    else if(b){
        loc[i] = now_pc(loc[i-1], b);
        b = 0;
    }
    else if(clear || compr || tixr){
        loc[i] = now_pc(loc[i-1], 2);
        clear = 0; compr = 0; tixr = 0;
    }
    else if(format4){
        loc[i] = now_pc(loc[i-1], 4);
        format4 = 0;
    }
    else loc[i] = now_pc(loc[i-1], 3);
}
int main()
{ 
    optab["CLEAR"] = "101101"; optab["COMP"] = "001010"; optab["J"] = "001111"; optab["JEQ"] = "001100";
    optab["JLT"] = "001110"; optab["JSUB"] = "010010"; optab["LDA"] = "000000"; optab["LDB"] = "011010";
    optab["LDCH"] = "010100"; optab["LDT"] = "011101"; optab["RD"] = "110110"; optab["STA"] = "000011";
    optab["STCH"] = "010101"; optab["STL"] = "000101"; optab["TD"] = "111000"; optab["TIXR"] = "101110";
    optab["WD"] = "110111";
    ifstream input("Fig2_5.txt");
    ofstream output("symbal.txt");
    ofstream obcode("object_code.txt");
    if(!input.is_open()) cout << "Error" << endl;
    // get the start location(前 1.2行)
    getline(input,temp);
    stringstream s1(temp);
    while(s1 >> temp) instruction.push_back(temp);
    loc[0] = instruction[2];
    name[0] = instruction[0];
    output << instruction[0] << " " << add_zero(loc[0], 4) << endl;
    loc[1] = loc[0];
    instruction.clear();
    getline(input,temp);
    stringstream s2(temp);
    while(s2 >> temp) instruction.push_back(temp);
    output << instruction[0] << " " << add_zero(loc[0], 4) << endl;
    format4 = 0; clear = 0; add = 0; compr = 0; b = 0; len = 0;
    while(getline(input, temp)){
        instruction.clear();
        stringstream ss(temp);
        // 把一行行指令拆開
        while(ss >> temp) instruction.push_back(temp);
        printf("i:%d clear:%d, format4:%d, add:%d", i, clear, format4, add);
        if(instruction[0] == "BASE" || instruction[0] == "END") continue;
        if(instruction.size() == 1){
            ins[i] = instruction[0];
            now_add(i);
            now_instruction(i);
        }
        if(instruction.size() == 3){
            // 指令
            if(instruction[1] != "BYTE" && instruction[1] != "RESW" && instruction[1] != "RESB"){
                name[i] = instruction[0];
                ins[i] = instruction[1];
                var[i] = instruction[2];
                now_add(i);
                now_instruction(i);
            }
            // 變數
            else{
                var[i] = instruction[0];
                type[i] = instruction[1];
                now_add(i);
                now_instruction(i);
            }
        }
        else if(instruction.size() == 2){
            ins[i] = instruction[0];
            var[i] = instruction[1];
            now_add(i);
            now_instruction(i); 
        }
        // SYMBAL
        if(name[i] != "") output << name[i] << " " << add_zero(loc[i], 4) << endl; 
        else if(type[i] != "") output << var[i] << " " << add_zero(loc[i], 4) << endl;
        // object code
        obcode << "H" << name[0] << "  " << add_zero(loc[0], 6) << endl;

        i++;
    }
    input.close();
    return 0;
}
