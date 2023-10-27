# Minesweeper

## Zadání
Implementujte hru Minessweeper s dálší funkcionalitou.
Hráč bude postupně zadávat součadnice umístění značky buď v režimu praporu pro oznacení bomb, 
nebo v normalním režimu pro otevření vybráných buněk.
Program automaticky vyhodnotí výsledek hry a tyto imformace zobrazí na terminál.

## Kompilace programu
Program lze zkompilovat následujícím příkazem:
```bash
g++ -std=c++17 main.cpp Window.cpp -o main -lpthread
```

## Spuštění a přepínače
Hra se spouští z příkazové řádky.
TO-DO

Příklady spuštění programu:
```bash
./main
```

## Ovládání programu
Pod hrací plochou je vypsáno, jaké souradnice zvolil hráč.

### Zadávání souřadnic
Souřadnice zadáváme jako dvojici `[řádek, sloupec]`. Řádky i sloupce jsou číslovány od 1.

### Ukončení programu
Program je ukončí automaticky, pokud hráč správně označí všechny bomby praporem. Hra končí i tehdy, pokud hráč zvolí souřadnice, 
které odpovidají bombě, totiž prohraje.

Program je možné kdykoliv ukončit klávesou `Q`.

## Testování programu
TO-DO
