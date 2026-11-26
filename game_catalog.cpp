#include <iostream>
#include <string>

using namespace std;

int main()
{
  int gameId[] = {};
  int n;

  cout << "Berapa angka?";
  cin >> n;

  gameId[n];

  for (int i = 0; i < 5; i++)
  {
    cout << "Masukkan angka ke-" << i + 1 << ": ";
    cin >> gameId[i];
  }

  cout << "\nIsi array:\n";
  for (int i = 0; i < 5; i++)
  {
    cout << gameId[i] << " ";
  }

  return 0;
}