#include <iostream>
#include <string>
using namespace std;

string permute(string &key, int *s, int k)
{
  string p(k, '0');
  for (int i = 0; i < k; i++)
  {
    p[i] = key[s[i] - 1];
  }
  return p;
}

void ls(string &s, int k)
{
  s = s.substr(k) + s.substr(0, k);
}

void generate_key(string &key, string &k1, string &k2, int *p_10, int *p8)
{
  string p = permute(key, p_10, 10);
  string lh = p.substr(0, 5);
  string rh = p.substr(5, 5);

  ls(lh, 1);
  ls(rh, 1);
  string p_combined = lh + rh;
  k1 = permute(p_combined, p8, 8);

  ls(lh, 1);
  ls(rh, 1);
  p_combined = lh + rh;
  k2 = permute(p_combined, p8, 8);

  cout << "K1: " << k1 << "  K2: " << k2 << endl;
}

string xor_op(string a, string b)
{
  string ans = "";
  for (int i = 0; i < a.size(); i++)
  {
    if (a[i] == b[i])
      ans += '0';
    else
      ans += '1';
  }
  return ans;
}

string sbox(string input, int box[4][4])
{
  int row = (input[0] - '0') * 2 + (input[3] - '0');
  int col = (input[1] - '0') * 2 + (input[2] - '0');
  int val = box[row][col];
  string out = "";
  if (val & 2)
    out += '1';
  else
    out += '0';

  if (val & 1)
    out += '1';
  else
    out += '0';

  return out;
}

string fk(string pt, string sk, int *ep, int *p4, int s0[4][4], int s1[4][4])
{
  string left = pt.substr(0, 4);
  string right = pt.substr(4, 4);

  string right_exp = permute(right, ep, 8);
  string xored = xor_op(right_exp, sk);
  string left_xor = xored.substr(0, 4);
  string right_xor = xored.substr(4, 4);

  string s0_out = sbox(left_xor, s0);
  string s1_out = sbox(right_xor, s1);

  string sbox_out = s0_out + s1_out;
  string p4_out = permute(sbox_out, p4, 4);

  string left_result = xor_op(left, p4_out);
  return left_result + right;
}

string encrypt(string pt, string k1, string k2, int *ip, int *ipi, int *ep, int *p4, int s0[4][4], int s1[4][4])
{
  string ipt = permute(pt, ip, 8);

  string temp = fk(ipt, k1, ep, p4, s0, s1);
  string swapped = temp.substr(4, 4) + temp.substr(0, 4);
  string final = fk(swapped, k2, ep, p4, s0, s1);

  string ct = permute(final, ipi, 8);
  return ct;
}

string decrypt(string ct, string k1, string k2, int *ip, int *ipi, int *ep, int *p4, int s0[4][4], int s1[4][4])
{
  string ipt = permute(ct, ip, 8);

  string temp = fk(ipt, k2, ep, p4, s0, s1); // Note K2 used first for decryption
  string swapped = temp.substr(4, 4) + temp.substr(0, 4);
  string final = fk(swapped, k1, ep, p4, s0, s1);

  string pt = permute(final, ipi, 8);
  return pt;
}

int main()
{
  string key;
  cout << "Enter 10-bit key ";
  cin >> key;

  int P_10[10] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
  int P_8[8] = {6, 3, 7, 4, 8, 5, 10, 9};
  int IP[8] = {2, 6, 3, 1, 4, 8, 5, 7};
  int IP_INV[8] = {4, 1, 3, 5, 7, 2, 8, 6};
  int EP[8] = {4, 1, 2, 3, 2, 3, 4, 1};
  int P4[4] = {2, 4, 3, 1};

  int S0[4][4] = {
      {1, 0, 3, 2},
      {3, 2, 1, 0},
      {0, 2, 1, 3},
      {3, 1, 3, 2}};
      
  int S1[4][4] = {
      {0, 1, 2, 3},
      {2, 0, 1, 2},
      {3, 0, 1, 0},
      {2, 1, 0, 3}};

  string k1(8, '0'), k2(8, '0');
  generate_key(key, k1, k2, P_10, P_8);

  string pt;
  cout << "Enter 8-bit plaintext: ";
  cin >> pt;

  string ct = encrypt(pt, k1, k2, IP, IP_INV, EP, P4, S0, S1);
  cout << "Encrypted ciphertext: " << ct << endl;

  string decrypted = decrypt(ct, k1, k2, IP, IP_INV, EP, P4, S0, S1);
  cout << "Decrypted plaintext:  " << decrypted << endl;

  return 0;
}
