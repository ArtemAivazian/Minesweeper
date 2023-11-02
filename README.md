# Minesweeper

## Zadání
Implementujte hru Minesweeper s dálší funkcionalitou:
hráč bude postupně zadávat součadnice umístění značky buď v režimu praporu pro oznacení bomb, 
nebo v normalním režimu pro otevření vybráných buněk.
Program automaticky vyhodnotí výsledek hry a tyto imformace zobrazí na terminál.

## Kompilace programu
Program lze zkompilovat následujícím příkazem:
```bash
g++ -std=c++17 main.cpp Window.cpp -o main -lpthread
```

Příklady spuštění programu:
```bash
./main
```

## Ovládání programu
Pod hrací plochou je vypsáno, jaké souradnice zvolil hráč.

### Změna režimu
Změnu režimu lze provest smačknutím klavesy `f`.
Následně hráč zadává souřadnice jako v normalním režimu.

### Zadávání souřadnic
Souřadnice zadáváme jako dvojici `řádek` MEZERA `sloupec` . Řádky i sloupce jsou číslovány od 1.
Pokud hráč chce odebrat prapor z buňky, musí zadat souřadnice teto buňky a prapor se vymaže.

### Ukončení programu
Program je ukončí automaticky, pokud hráč správně označí všechny bomby praporem:
`You won!`

Hra končí i tehdy, pokud hráč zvolí souřadnice, 
které odpovidají bombě, totiž prohraje.
`You lost!`

Program je možné kdykoliv ukončit klávesou `q`.

## Testování programu
TO-DO
