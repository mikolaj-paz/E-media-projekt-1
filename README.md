# Format PNG - Projekty 1 i 2
Ten projekt zawiera obie aplikacje projektowe.
Wszystkie zewnętrzne biblioteki pobierane są jako *git submodules*, a konfiguracja oraz kompilacja wykonywane są przy pomocy **CMake**.

## Klonowanie repozytorium
Klonowanie razem z podmodułami
```bash
git clone --recurse-submodules https://github.com/mikolaj-paz/E-media-projekt-1
```
Dodawanie modułów do gotowego repozytorium
```bash
git submodule update --init --recursive
```
> [!CAUTION]
> W momencie tworzenia projektu najnowsza wersja **cryptopp-cmake** działa niepoprawnie. W projekcie wykorzystywałem commit z następującym SHA: ```00a151f8489daaa32434ab1f340e6750793ddf0c```.

## Kompilacja
```bash
mkdir build
cd build
cmake ..
cmake --build .
```
W folderze ```/build``` powinny znaleźć się dwa pliki wykonawcze ```project1.exe``` i ```project2.exe```.
