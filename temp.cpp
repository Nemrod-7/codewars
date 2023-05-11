#include<bits/stdc++.h>
using namespace std;

const int mod = 1e9+7;
long long ans[100];

inline int add(long long a,long long b) {a%=mod, b%=mod;return (a+b+mod)%mod;}
inline int mul(long long a,long long b) {return a*b%mod;}

inline int qpow(long long a, long long b) {/*{{{*/
	int ret = 1;
	while(b) {
		if(b&1) ret = mul(ret, a);
		a = mul(a,a);
		b>>=1;
	}
	return ret;
}

inline int inv(long long a) { return qpow(a, mod-2); }

long long fib[100];

long long S(long long a) {
	long long ret = 0;
	ret = add(ret, mul(45, mul(inv(9), add(qpow(10, a/9), -1))));
	long long buf = a%9+1;
	ret = add(ret, mul(mul(buf, buf+1), mul(inv(2), qpow(10, a/9))));
	ret = add(ret, -a-1);
	return ret;
}
void init_fib() {
	fib[1] = 1;
	// cout<<"f_1 = "<<fib[1]<<endl;
	for(int i = 2;i<=90;i++) {
		fib[i] = fib[i-1] + fib[i-2];
		// cout<<"f_"<<i<<" = "<<fib[i]<<endl;
	}
}
int main() {
	init_fib();
	for(int i = 2;i<=90;i++) {
		ans[i] = add(ans[i-1],S(fib[i]));
		cout<<"sum of S(f_"<<i<<") = "<<ans[i]<<endl;
    // cout << S(fib[i]) << "\n";
	}
  for(int i = 1;i<=20;i++) {
    // ans[i] = add(ans[i-1],S(fib[i]));
    // cout<<"sum of S(f_"<<i<<") = "<<ans[i]<<endl;
    cout << S (i) << "\n";
  }
	return 0;
}
