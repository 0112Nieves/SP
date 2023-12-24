#include<bits/stdc++.h>
#define print(i) cout << #i << " is " << i << endl;
#define printLoop(l, n) for(int i = 0; i < n; i++) cout << #l << i << ' ' << l[i] << endl;
using namespace std;
string temp;
vector<string> instruction;
bool format4, clear, compr, tixr, ba;
map<string, string> optab;
map<string, string>::iterator it; 
string m_record[20];
ofstream output("symbal.txt");
ofstream obcode("object_code.txt");
vector<pair<string, string>> symbal;
string loc[100], name[100], ins[100], var[100], type[100], code[200], disp[200], var1[100], base, result[200], B_register, store[100];
int add = 0, len = 0, b = 0, line;
// 把它補成4位
string add_zero(string str, int n) {
    while (str.length() < n) {
        str = "0" + str;
    }
    return str;
}
// 10進換16進
string turn_hex(int n){
    // 使用 std::stringstream 将长整数转换为16进制字符串
    stringstream hexStream;
    hexStream << hex << n;
	string str = hexStream.str();
	for(auto &ch:str){
		if(islower(ch))
			ch -= ('a' - 'A');
	}
    return str;
}
// 16進換10進
int turn_decimal(const string loc){
    long long int ans = 0;
    for(int k = 0; k < loc.length(); k++){
        int tmp;
        if(isdigit(loc[k])){
            tmp = loc[k]-'0';
        }
        else{
            switch(loc[k]){
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
    else if(ins[i] == "BASE") ba = 1;
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
    else if(ba){
        ba = 0;
        loc[i] = to_string(stoi(now_pc(loc[i-1], 0)) - 3);
    }
    else loc[i] = now_pc(loc[i-1], 3);
}
void init(){
    optab["CLEAR"] = "B4"; optab["COMPR"] = "A0"; optab["J"] = "001111"; optab["JEQ"] = "001100";
    optab["JLT"] = "001110"; optab["JSUB"] = "010010"; optab["LDA"] = "000000"; optab["LDB"] = "011010";
    optab["LDCH"] = "010100"; optab["LDT"] = "011101"; optab["RD"] = "110110"; optab["STA"] = "000011";
    optab["STCH"] = "010101"; optab["STL"] = "000101"; optab["TD"] = "111000"; optab["WD"] = "110111";
    optab["COMP"] = "001010"; optab["STX"] = "000100";
}
char which_register(string in){
    char a;
    if(in == "A") a =  '0';
    else if(in == "X") a = '1';
    else if(in == "B") a = '3';
    else if(in == "S") a = '4';
    else if(in == "T") a = '5';
    return a;
}
string decimalToBinary(int n) {
    bitset<32> b(n);
    string s = b.to_string();
    for(int i=0;i<s.size();i++) {
        if(s[i]!='0')
            return s.substr(i,s.size()-i);
    }
    return s;
}
int main()
{ 
    init();
    string file;
    cout << "Please input the file name:";
    cin >> file;
    ifstream input(file);
    if(!input.is_open() || !output.is_open() || !obcode.is_open()){
        cout << "Error" << endl;
        return 1;
    }
    // get the start location(前 1.2行)
    getline(input,temp);
    stringstream s1(temp);
    while(s1 >> temp) instruction.push_back(temp);
    loc[0] = instruction[2];
    ins[0] = instruction[1];
    name[0] = instruction[0];
    output << instruction[0] << " " << add_zero(loc[0], 4) << endl;
    loc[1] = loc[0];
    instruction.clear();
    getline(input,temp);
    stringstream s2(temp);
    while(s2 >> temp) instruction.push_back(temp);
    var[1] = instruction[2];
    ins[1] = instruction[1];
    output << instruction[0] << " " << add_zero(loc[0], 4) << endl;
    int i = 2, flag = 0;
    format4 = 0; clear = 0; add = 0; compr = 0; b = 0; len = 0;
    while(getline(input, temp)){
        instruction.clear();
        stringstream ss(temp);
        // 把一行行指令拆開
        while(ss >> temp) instruction.push_back(temp);
        if(instruction[0] == "END"){
            continue;
        }
        if(instruction[0] == "BASE"){
            flag = i;
            ins[i] = instruction[0];
            var[i] = instruction[1];
            base = instruction[1];
            now_add(i);
            now_instruction(i);
            i++;
            continue;
        }
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
                var1[i] = instruction[0];
                type[i] = instruction[1];
                store[i] = instruction[2];
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
        if(name[i] != ""){
            output << name[i] << " " << add_zero(loc[i], 4) << endl;
            symbal.push_back(make_pair(name[i], loc[i]));
        }
        else if(type[i] != ""){
            output << var1[i] << " " << add_zero(loc[i], 4) << endl;
            symbal.push_back(make_pair(var1[i], loc[i]));
        }
        i++;
    }
    // object code
    int j = 1, r = 0, total = i;

    string var_loc[200], final[10], record_loc[10];
    bool no_out[10];
    memset(no_out, 0, sizeof(no_out));
    obcode << "H" << name[0] << "  " << add_zero(loc[0], 6) << add_zero(turn_hex(stoi(loc[i-1], NULL, 16)+1), 6) << endl;
    record_loc[0] = loc[0];
    while(j != total){
        int byte;
        char n = '0', i = '0', x = '0', b = '0', p = '0', e = '0';
        bool immediate = false, indirect = false, jump = false, digit = false;
        format4 = 0;
        // 特殊處理
        if(type[j] == "RESW" || type[j] == "RESB") jump = true;
        if(ins[j] == "BASE" || j == flag){
            for(const auto& pair : symbal){
                if(base == pair.first){
                    B_register = pair.second;
                    var_loc[j] = pair.second;
                    break;
                }
            }
            jump = true;
        }
        if(ins[j] == "TIXR"){
            result[j] = "B850";
            jump = true;
        }
        if(ins[j] == "CLEAR"){
            result[j] += optab[ins[j]];
            result[j] += which_register(var[j]);
            result[j] += '0';
            jump = true;
        }
        if(ins[j] == "COMPR"){
            result[j] += optab[ins[j]];
            result[j] += which_register(var[j].substr(0, 1));
            result[j] += which_register(var[j].substr(2, 1));
            jump = true;
        }
        if(type[j] == "BYTE"){
            if(store[j][0] == 'C'){
                string tmp = store[j].substr(2, store[j].size() - 3);
                for(int k = 0; k < tmp.size(); k++){
                    int t = int(tmp[k]);
                    result[j] += turn_hex(t);
                }
            }
            else if(store[j][0] == 'X') result[j] += store[j].substr(2, store[j].size() - 3);
            jump = true;
        }
        if(type[j] == "WORD"){
            result[j] = add_zero(turn_hex(stoi(var[j])), 6);
            jump = true;
        }
        if(ins[j] == "RSUB"){
            result[j] = "4F0000";
            jump = true;
        }
        //obcode << result[j] << endl;
        final[line] += result[j];
        if(jump){
            jump = false;
            j++;
            continue;
        }
        // format 4
        if(ins[j][0] == '+'){
            if(isdigit(var[j][1])) no_out[r] = true;
            e = '1';
            format4 = 1;
            ins[j].erase(ins[j].begin());
            m_record[r] = loc[j];
            r++;
        }
        code[j] += optab[ins[j]];
        // immediate addressing
        if(var[j][0] == '#'){
            i = '1';
            immediate = true;
        }
        // indirect addressing
        else if(var[j][0] == '@'){
            n = '1';
            indirect = true;
        }
        // index addressing
        if(var[j].find(',') != string::npos){
            x = '1';
            string temp = "";
            for(auto ch:var[j]){
                if(ch == ',') break;
                temp += ch;
            }
            var[j] = temp;
        }
        // 找disp的值
        if(immediate || indirect){
            if(isdigit(var[j][1])){
                string tmp;
                var_loc[j] = var[j].substr(1, var[j].length() - 1);
                digit = true;
            }
            else{
                for(const auto& pair : symbal){
                    if(var[j].substr(1, var[j].length()-1) == pair.first){
                        var_loc[j] = pair.second;
                        break;
                    }
                }
            }
        }
        else{
            for(const auto& pair : symbal){
                if(var[j] == pair.first){
                    var_loc[j] = pair.second;
                    break;
                }
            }
        }
        if(!immediate && !indirect){
            n = '1'; i = '1';
        }
        //pc relative
        if(stoi(var_loc[j], NULL, 16) - stoi(loc[j+1], NULL, 16) <= 2047 && stoi(var_loc[j], NULL, 16) - stoi(loc[j+1], NULL, 16) >= -2048 && j+1 < i && !digit){
            if(format4){
                bitset<20> d(stoi(var_loc[j], NULL, 16) - stoi(loc[j+1], NULL, 16));
                disp[j] = d.to_string();
            }
            else if(!digit){
                bitset<12> d(stoi(var_loc[j], NULL, 16) - stoi(loc[j+1], NULL, 16));
                disp[j] = d.to_string();
            }
            p = '1';
        }
        //base relative
        else{
            string tmp;
            for(int k = 0; k < i; k++){
                if(var1[k] == base){
                    tmp = var_loc[j];
                    break;
                }
            }
            b = '1';
            if(format4){
                bitset<20> d(stoi(tmp, NULL, 16) - stoi(B_register, NULL, 16));
                disp[j] = d.to_string();
            }
            else{
                bitset<12> d(stoi(var_loc[j], NULL, 16) - stoi(B_register, NULL, 16));
                disp[j] = d.to_string();
            }
        }
        if(digit || format4){
            b = '0'; p = '0';
            code[j] += n; code[j] += i; code[j] += x; code[j] += b; code[j] += p; code[j] += e;
            if(!format4) code[j] += add_zero(var_loc[j], 12);
        }
        else if(!format4){
            code[j] += n; code[j] += i; code[j] += x; code[j] += b; code[j] += p; code[j] += e; code[j] += disp[j];
        }
        // 4位變一16進位的數
        int cnt = 0, num = 0;
        for(int k = 0; k <= code[j].length()-1; k++){
            num *= 2;
            num += (code[j][k] - '0');
            cnt++;
            if(cnt == 4){
                cnt = 0;
                result[j] += turn_hex(num);
                num = 0;
            }
        }
        if(format4){
            if(digit) result[j] += add_zero(turn_hex(stoi(var_loc[j])), 5);
            else result[j] += add_zero(var_loc[j], 5);
        }
        //obcode << result[j] << endl;
        final[line] += result[j];
        if(type[j+1] == "RESW" || type[j+1] == "RESB" || final[line].length() / 2 >= 29){
            obcode << "T" << add_zero(record_loc[line], 6) << add_zero(turn_hex(final[line].length()/2), 2) << final[line] << endl;
            line++;
            record_loc[line] = loc[j+1];
        }
        j++;
    }
    obcode << "T" << add_zero(record_loc[line], 6) << add_zero(turn_hex(final[line].length()/2), 2) << final[line] << endl;
    for(int i = 0; i < r; i++){
        if(!no_out[i]) obcode << "M" << add_zero(to_string(stoi(m_record[i])+1), 6) << "05" << endl;
    }
    obcode << "E" << add_zero(loc[0], 6) << endl;
    input.close();
    return 0;
}
