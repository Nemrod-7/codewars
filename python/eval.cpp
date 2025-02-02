#include <bits/stdc++.h>

using namespace std;

int precedence (char op){
    if (op == '+'||op == '-') return 1;
    if (op == '*'||op == '/') return 2;
    return 0;
}

int operate (double a, double b, char op){
    switch(op){
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
    }
    return 0;
}

double evaluate(string tokens){

    stack<double> value;
    stack<char> oper;

    for (int i = 0; i < tokens.size(); i++){

        if (tokens[i] == ' ') {
            continue;
        } else if(tokens[i] == '('){
            oper.push(tokens[i]);
        } else if (isdigit(tokens[i])){
            double val = 0;

            while (i < tokens.length() && isdigit(tokens[i])) {
                val = (val*10) + (tokens[i]-'0');
                i++;
            }

            value.push(val);

            i--;
        } else if (tokens[i] == ')') { // Closing brace encountered, solve entire brace.
            while (!oper.empty() && oper.top() != '(') {
                double val2 = value.top(); value.pop();
                double val1 = value.top(); value.pop();
                char op = oper.top(); oper.pop();

                value.push(operate(val1, val2, op));
            }

            if (!oper.empty()) oper.pop(); // pop opening brace.
        } else  {
            while (!oper.empty() && precedence(oper.top()) >= precedence(tokens[i])){
                double val2 = value.top(); value.pop();
                double val1 = value.top(); value.pop();
                char op = oper.top(); oper.pop();

                value.push(operate(val1, val2, op));
            }

            oper.push(tokens[i]);
        }
    }

    while(!oper.empty()){
        double val2 = value.top(); value.pop();
        double val1 = value.top(); value.pop();
        char op = oper.top(); oper.pop();

        value.push(operate(val1, val2, op));
    }

    return value.top();
}


int main () {

    cout << evaluate("7 - ((6 / 3) - 2)");
}
