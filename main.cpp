#include <iostream>
#include <fstream>
#include <queue>
#include <regex>
#include <unordered_map>
#include <string>
#include <bits/stdc++.h>
#include <algorithm>
#include <utility>

using namespace std;

void pass_1(const string &file);

bool comment_line(const string &line);

string get_operand(string line);

void print_line_at_file(const string &line);

int length_of_ins(const string &operation, string line);

string has_label(string line);

string get_operation(string line);

string get_comment(string line);

bool validate_arr(const string &operation, string line);

bool validate_dir(const string &operation, string line);

int getInstructionLength(const string &operation, string line);

void print_sym_table(const string &label, int pcCounter);

void sym_table();

void print_format(const string &line, int pcCounter, int no_of_line);

bool has_plus(string line);

string trim(const string &str);

struct arr {
    string opCode;
    string length;
    string rgx;
};

unordered_map<string, struct arr> opt_table;
unordered_map<string, string> directives;
unordered_map<string, string> symtab;
string general_regex = R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(\+)?([a-z]{1,6})\s*(\s(#|@)[a-z][a-z0-9]{0,7}|\s[a-z][a-z0-9]{0,7}(,x)?|\s(#|@)[0-9]{1,4}|\s[0-9]{1,4}(,x)?|\s([axlbstf]|pc|sw)\s*,\s*([axlbstf]|pc|sw)|\s[a-z][a-z0-9]{0,7}|\s-?[0-9]{1,4}|\s-?[1-9]{1,4}(,-?[1-9]{1,4}){0,2}|c'.{1,15}'|x'(\d|a|b|c|d|e|f){1,2}'|\*)(\s*|\s+\..{0,31})$|^\s*(start)(\s*|\s+\..{0,31})$|^\s*(end)(\s*|\s+\..{0,31})$|^\s*(rsub)(\s*|\s+\..{0,31})$)";

bool validate_dir(const string &operation, string line);

int main() {
    remove("output.txt");
    ///Todo
    cout << "Enter pass:" << endl;
    string pass;
    cin >> pass;
    cout << "Enter file path:" << endl;
    string file_path;
    cin >> file_path;

    struct arr temp = {"18", "3",
                       R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(\+)?add\s+((#|@)[a-z][a-z0-9]{0,7}|[a-z][a-z0-9]{0,7}(,x)?|(#|@)[0-9]{1,4}|[0-9]{1,4}(,x)?)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("add", temp);
    temp = {"1c", "3",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(\+)?sub\s+((#|@)[a-z][a-z0-9]{0,7}|[a-z][a-z0-9]{0,7}(,x)?|(#|@)[0-9]{1,4}|[0-9]{1,4}(,x)?)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("sub", temp);
    temp = {"28", "3",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(\+)?comp\s+((#|@)[a-z][a-z0-9]{0,7}|[a-z][a-z0-9]{0,7}(,x)?|(#|@)[0-9]{1,4}|[0-9]{1,4}(,x)?)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("comp", temp);
    temp = {"00", "3",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(\+)?lda\s+((#|@)[a-z][a-z0-9]{0,7}|[a-z][a-z0-9]{0,7}(,x)?|(#|@)[0-9]{1,4}|[0-9]{1,4}(,x)?)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("lda", temp);
    temp = {"68", "3",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(\+)?ldb\s+((#|@)[a-z][a-z0-9]{0,7}|[a-z][a-z0-9]{0,7}(,x)?|(#|@)[0-9]{1,4}|[0-9]{1,4}(,x)?)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("ldb", temp);
    temp = {"6c", "3",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(\+)?lds\s+((#|@)[a-z][a-z0-9]{0,7}|[a-z][a-z0-9]{0,7}(,x)?|(#|@)[0-9]{1,4}|[0-9]{1,4}(,x)?)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("lds", temp);
    temp = {"74", "3",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(\+)?ldt\s+((#|@)[a-z][a-z0-9]{0,7}|[a-z][a-z0-9]{0,7}(,x)?|(#|@)[0-9]{1,4}|[0-9]{1,4}(,x)?)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("ldt", temp);
    temp = {"08", "3",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(\+)?ldl\s+((#|@)[a-z][a-z0-9]{0,7}|[a-z][a-z0-9]{0,7}(,x)?|(#|@)[0-9]{1,4}|[0-9]{1,4}(,x)?)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("ldl", temp);
    temp = {"04", "3",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(\+)?ldx\s+((#|@)[a-z][a-z0-9]{0,7}|[a-z][a-z0-9]{0,7}(,x)?|(#|@)[0-9]{1,4}|[0-9]{1,4}(,x)?)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("ldx", temp);
    temp = {"50", "3",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(\+)?ldch\s+((#|@)[a-z][a-z0-9]{0,7}|[a-z][a-z0-9]{0,7}(,x)?|(#|@)[0-9]{1,4}|[0-9]{1,4}(,x)?)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("ldch", temp);
    temp = {"70", "3",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(\+)?ldf\s+((#|@)[a-z][a-z0-9]{0,7}|[a-z][a-z0-9]{0,7}(,x)?|(#|@)[0-9]{1,4}|[0-9]{1,4}(,x)?)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("ldf", temp);
    temp = {"0c", "3",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(\+)?sta\s+((#|@)[a-z][a-z0-9]{0,7}|[a-z][a-z0-9]{0,7}(,x)?|(#|@)[0-9]{1,4}|[0-9]{1,4}(,x)?)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("sta", temp);
    temp = {"78", "3",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(\+)?stb\s+((#|@)[a-z][a-z0-9]{0,7}|[a-z][a-z0-9]{0,7}(,x)?|(#|@)[0-9]{1,4}|[0-9]{1,4}(,x)?)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("stb", temp);
    temp = {"54", "3",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(\+)?stch\s+((#|@)[a-z][a-z0-9]{0,7}|[a-z][a-z0-9]{0,7}(,x)?|(#|@)[0-9]{1,4}|[0-9]{1,4}(,x)?)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("stch", temp);
    temp = {"80", "3",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(\+)?stf\s+((#|@)[a-z][a-z0-9]{0,7}|[a-z][a-z0-9]{0,7}(,x)?|(#|@)[0-9]{1,4}|[0-9]{1,4}(,x)?)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("stf", temp);
    temp = {"14", "3",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(\+)?st\s+((#|@)[a-z][a-z0-9]{0,7}|[a-z][a-z0-9]{0,7}(,x)?|(#|@)[0-9]{1,4}|[0-9]{1,4}(,x)?)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("st", temp);
    temp = {"7c", "3",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(\+)?sts\s+((#|@)[a-z][a-z0-9]{0,7}|[a-z][a-z0-9]{0,7}(,x)?|(#|@)[0-9]{1,4}|[0-9]{1,4}(,x)?)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("sts", temp);
    temp = {"e8", "3",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(\+)?stsw\s+((#|@)[a-z][a-z0-9]{0,7}|[a-z][a-z0-9]{0,7}(,x)?|(#|@)[0-9]{1,4}|[0-9]{1,4}(,x)?)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("stsw", temp);
    temp = {"84", "3",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(\+)?stt\s+((#|@)[a-z][a-z0-9]{0,7}|[a-z][a-z0-9]{0,7}(,x)?|(#|@)[0-9]{1,4}|[0-9]{1,4}(,x)?)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("stt", temp);
    temp = {"10", "3",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(\+)?stx\s+((#|@)[a-z][a-z0-9]{0,7}|[a-z][a-z0-9]{0,7}(,x)?|(#|@)[0-9]{1,4}|[0-9]{1,4}(,x)?)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("stx", temp);
//* wel operands eh

    temp = {"2c", "3",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(\+)?tix\s+((#|@)[a-z][a-z0-9]{0,7}|[a-z][a-z0-9]{0,7}(,x)?|(#|@)[0-9]{1,4}|[0-9]{1,4}(,x)?)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("tix", temp);
    temp = {"3c", "3",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(\+)?j\s+((#|@)[a-z][a-z0-9]{0,7}|[a-z][a-z0-9]{0,7}(,x)?|(#|@)[0-9]{1,4}|[0-9]{1,4}(,x)?|\*)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("j", temp);
    temp = {"3c", "3",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(\+)?jeq\s+((#|@)[a-z][a-z0-9]{0,7}|[a-z][a-z0-9]{0,7}(,x)?|(#|@)[0-9]{1,4}|[0-9]{1,4}(,x)?|\*)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("jeq", temp);
    temp = {"34", "3",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(\+)?jlt\s+((#|@)[a-z][a-z0-9]{0,7}|[a-z][a-z0-9]{0,7}(,x)?|(#|@)[0-9]{1,4}|[0-9]{1,4}(,x)?|\*)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("jlt", temp);
    temp = {"38", "3",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(\+)?jgt\s+((#|@)[a-z][a-z0-9]{0,7}|[a-z][a-z0-9]{0,7}(,x)?|(#|@)[0-9]{1,4}|[0-9]{1,4}(,x)?|\*)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("jgt", temp);
//type 2 opers
    temp = {"90", "2",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*addr\s+([axlbstf]|pc|sw)\s*,\s*([axlbstf]|pc|sw)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("addr", temp);
    temp = {"94", "2",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*subr\s+([axlbstf]|pc|sw)\s*,\s*([axlbstf]|pc|sw)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("subr", temp);
    temp = {"ac", "2",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*rmo\s+([axlbstf]|pc|sw)\s*,\s*([axlbstf]|pc|sw)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("rmo", temp);
    temp = {"a0", "2",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*compr\s+([axlbstf]|pc|sw)\s*,\s*([axlbstf]|pc|sw)(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("compr", temp);
//register wa7ed bas hena
    temp = {"b8", "2",
            R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*tixr\s+([axlbstf]|pc|sw)\s*(\s*|\s+\..{0,31})$)"
    };
    opt_table.emplace("tixr", temp);

    directives.emplace("start",
                       R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(start)(\s+(\d|a|b|c|d|e|f){1,4})?(\s*|\s+\..{0,31})$)");
    directives.emplace("end",
                       R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(end)(\s*|\s+\..{0,31})$)");
    directives.emplace("byte",
                       R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(byte)\s+(c'.{1,15}'|x'(\d|a|b|c|d|e|f){1,2}')(\s*|\s+\..{0,31})$)");
    directives.emplace("word",
                       R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(word)\s+((@|#)?[a-z][a-z0-9]{0,7}|(@|#)?-?[0-9]{1,4}|(@|#)?-?[1-9]{1,4}(,-?[1-9]{1,4}){0,2})?(\s*|\s+\..{0,31})$)");
    directives.emplace("resb",
                       R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(resb)(\s+[0-9]{1,4})(\s*|\s+\..{0,31})$)");
    directives.emplace("resw",
                       R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(resw)(\s+[0-9]{1,3}|\s+[1-3][0-9]{1,3})(\s*|\s+\..{0,31})$)");
    directives.emplace("org",
                       R"(^\s*(org)\s+(@?[a-z][a-z0-9]{0,7}(,x)?)(\s*|\s+\..{0,31})$ )");
    directives.emplace("equ",
                       R"(^\s*([a-z][a-z0-9]{0,7}\s)?\s*(equ)\s+(@?[a-z][a-z0-9]{0,7}(,x)?)(\s*|\s+\..{0,31})$)");
    directives.emplace("base",
                       R"(^\s*(base)\s+(@?[a-z][a-z0-9]{0,7}(,x)?|\*)(\s*|\s+\..{0,31})$)");
    directives.emplace("nobase", "^\\s*(nobase)\\s*$");

    if (pass == "pass_1") {
        pass_1(file_path);
        sym_table();
    }

    return 0;

}

bool error_flag = false;

void pass_1(const string &file) {
    ifstream infile(file.c_str());
    string line;
    int starting_adress = 0;
    int pcCounter = 0;
    int count = 0;
    string error;
    string warning;
    bool warning_flag = false;
    int instruction_length = 0;
    int no_of_line = 0;
    while (getline(infile, line)) {
        no_of_line++;
        if (!comment_line(line)) {
            error_flag = false;
            string label = has_label(line);
            string operation = get_operation(line);
            struct arr obj1 = opt_table[operation];
            string dir = directives[operation];
            string operand = get_operand(line);

            if (operation == "end") {
                pcCounter += instruction_length;
                print_format(line, pcCounter, no_of_line);
                return;
            } else {
                if (operation == "start") {
                    if (count != 0) {
                        error = "***start is not in the first line***";
                        error_flag = true;
                    } else {
                        pcCounter = stoi(operand, nullptr, 10);
                        starting_adress = pcCounter;
                        count++;
                        error = "";
                    }
                } else {
                    if (obj1.opCode != "") {
                        if (validate_arr(operation, line)) {
                            if (count > 1) {
                                pcCounter += instruction_length;
                            }
                            instruction_length = length_of_ins(operation, line);
                            count++;
                        } else {
                            error = "invalid instruction syntax";
                            error_flag = true;
                        }
                    } else if (!dir.empty()) {
                        if (validate_dir(operation, line)) {
                            if (count > 1) {
                                pcCounter += instruction_length;

                            }
                            instruction_length = getInstructionLength(
                                    operation, line);
                            if (instruction_length == 0) {
                                warning_flag = true;
                                warning = "**don't use this directive.**";
                            }
                            count++;
                        } else {
                            error = "invalid directive syntax";
                            error_flag = true;
                        }
                    } else {
                        error = "***opcode doesn't exist.***";
                        error_flag = true;
                        count++;
                    }

                }
                print_format(line, pcCounter, no_of_line);

                if (error_flag) {
                    print_line_at_file(error);
                }
                if (warning_flag) {
                    print_line_at_file(warning);
                }
                error_flag = false;
                warning_flag = false;
            }

            if (!label.empty()) {
                if (symtab.count(label) != 0) {
                    error = "***label has been defined.***";
                    error_flag = true;
                } else {
                    symtab.emplace(label, to_string(pcCounter));
                }
            }

        } else {
            print_format(line, pcCounter, no_of_line);
        }

    }

    int program_length = pcCounter - starting_adress;
}

bool comment_line(const string &line) {
    regex reg("^\\s*\\..*$");
    if(regex_match(line, reg))
        return true;
    regex re("\\s*");
    return regex_match(line, re);
}


string get_operand(string line) {
    std::transform(line.begin(), line.end(), line.begin(), ::tolower);
    string operand;
    regex greg(general_regex);
    if (regex_match(line, greg)) {
        smatch matches;
        if (regex_search(line, matches, greg) && matches.size() > 1) {
            return matches[4];
        }
    }


    return "";
}

void print_line_at_file(const string &line) {
    ofstream file;
    file.open("./output.txt", std::ios_base::app);
    file << line << endl;
    file.close();
}

int length_of_ins(const string &operation, string line) {

    struct arr value = opt_table[operation];
    int length = stoi(value.length, nullptr, 10);
    if (length == 3 && has_plus(std::move(line))) {
        length = 4;
    }
    return length;
}

string has_label(string line) {
    std::transform(line.begin(), line.end(), line.begin(), ::tolower);
    string label;
    std::smatch match;
    regex greg(general_regex);
    try {
        if (regex_search(line, match, greg) && match.size() > 1) {
            label = match.str(1);
        } else {
            label = std::string("");
        }
    }
    catch (std::regex_error &e) {
        // Syntax error in the regular expression
    }
    return label;
}

string get_operation(string line) {
    std::transform(line.begin(), line.end(), line.begin(), ::tolower);
    string oper;
    std::smatch match;
    regex greg(general_regex);
    try {
        if (regex_search(line, match, greg) && match.size() > 1) {
            oper = match.str(3) + match.str(14) + match.str(16) + match.str(18);
        } else {
            oper = std::string("");
        }
    }
    catch (std::regex_error &e) {
        // Syntax error in the regular expression
    }
    return oper;

}

bool validate_arr(const string &operation, string line) {
    struct arr obj = opt_table[operation];
    std::transform(line.begin(), line.end(), line.begin(), ::tolower);
    string Regex = obj.rgx;
    regex reg(Regex);
    return regex_match(line, reg);
}

bool validate_dir(const string &operation, string line) {
    string obj = directives[operation];
    std::transform(line.begin(), line.end(), line.begin(), ::tolower);
    const string &Regex = obj;
    regex reg(Regex);
    return regex_match(line, reg);
}

int getInstructionLength(const string &operation, string line) {
    int l = 0;
    if (operation == "word") {
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        std::smatch match;
        regex e(directives[operation]);
        try {
            if (std::regex_search(line, match, e) && match.size() > 1) {
                string string1 = match.str(3);
                l = (count(string1.begin(), string1.begin() + string1.length(),
                           ',') + 1) * 3;
            }
        }
        catch (std::regex_error &e) {
            // Syntax error in the regular expression
        }

    } else if (operation == "byte") {
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        std::smatch match;
        std::regex r(directives[operation]);
        try {
            if (regex_search(line, match, r) && match.size() > 1 &&
                match.str(3)[0] == 'c')
                l = match.str(3).size() - 3;
            else if (regex_search(line, match, r) && match.size() > 1)
                l = 1;
        }
        catch (std::regex_error &e) {
            // Syntax error in the regular expression
        }
    } else if (operation == "resw") {
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        std::smatch match;
        std::regex r(directives[operation]);
        try {
            if (regex_search(line, match, r) && match.size() > 1)
                l = stoi(match.str(3), nullptr, 10) * 3;
        }
        catch (std::regex_error &e) {
            // Syntax error in the regular expression
        }
    } else if (operation == "resb") {
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        std::smatch match;
        std::regex r(directives[operation]);
        try {
            if (std::regex_search(line, match, r) && match.size() > 1)
                l = stoi(match.str(3), nullptr, 10);
        }
        catch (std::regex_error &e) {
            // Syntax error in the regular expression
        }
    } else {
        l = 0;
    }
    ///set warning.
    return l;
}

void sym_table() {
    remove("sym.txt");
    string line;
    for (auto x : symtab) {
        string label;
        label += x.first;
        print_sym_table(label, stoi(x.second));
    }
}

void print_sym_table(const string &label, int pcCounter) {
    FILE *pFile;
    pFile = fopen("sym.txt", "a");
    string t = trim(label);
    char l[t.size()];
    strcpy(l, t.c_str());
    fprintf(pFile, "%06x %-8s\n", pcCounter, l);
    fclose(pFile);
}

bool has_plus(string line) {
    std::transform(line.begin(), line.end(), line.begin(), ::tolower);
    std::smatch match;
    regex greg(general_regex);
    string plus_sign;
    try {
        if (regex_search(line, match, greg) && match.size() > 1) {
            plus_sign = match.str(2);
        } else {
            plus_sign = std::string("");
        }
    }
    catch (std::regex_error &e) {
        // Syntax error in the regular expression
    }
    return plus_sign == "+";
}

string get_comment(string line) {
    std::transform(line.begin(), line.end(), line.begin(), ::tolower);
    regex greg(general_regex);
    if (regex_match(line, greg)) {
        smatch matches;
        if (regex_search(line, matches, greg) && matches.size() > 1) {
            string a = matches[13];
            a += matches[15];
            a += matches[17];
            a += matches[19];
            return (a);
        }
    }
    return "";
}

void print_format(const string &line, int pcCounter, int no_of_line) {
    FILE *pFile;
    pFile = fopen("./output.txt", "a");
    string t;
        if (!comment_line(line) && !error_flag) {
            string oper;
            if (has_plus(line)) {
                oper += "+";
            }
            oper += trim(get_operation(line));

            t = trim(has_label(line));
            char l[t.size() + 1];
            strcpy(l, t.c_str());

            t = trim(get_comment(line));
            char c[t.size() + 1];
            strcpy(c, t.c_str());

            char opera[oper.size() + 1];
            strcpy(opera, oper.c_str());

            t = trim(get_operand(line));
            char o[t.size() + 1];
            strcpy(o, t.c_str());

            fprintf(pFile, "%03d %06x %-8s %-6s %-18s %-31s\n", no_of_line,
                    pcCounter, l,
                    opera, o, c);
        } else {
            t = trim(line);
            char c[t.size()];
            strcpy(c, t.c_str());
            fprintf(pFile, "%03d %06x %-40s\n", no_of_line, pcCounter, c);
        }
    fclose(pFile);
}

string trim(const string &str) {
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

