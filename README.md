# Minesweeper

## Zadání
Implementujte hru Minesweeper s dálší funkcionalitou:
hráč bude postupně zadávat součadnice umístění značky buď v režimu praporu pro oznacení bomb, 
nebo v normalním režimu pro otevření vybráných buněk.
Program automaticky vyhodnotí výsledek hry a tyto imformace zobrazí na terminál.
Je potřebá zařidit uložeí hry.

## Kompilace programu
Program lze zkompilovat následujícím příkazem:
```bash
g++ -std=c++17 -o main main.cpp Window.cpp -lpthread -I/json.hpp
```

Příklady spuštění programu:
```bash
./main
```

## Ovládání programu

### Uložení a stahování hry

Pokud hráč už hrál a ukončil hru, hra už byla uložena do JSON souboru `grids.json`. Při dálším spůštění programu, hráč bude muset vybrat,
zda on chce pokračovat hru s uloženými nastaveními nebo začít znova od začátku.
`You have saved progress. Do you want to continue? y/n:`

### Změna režimu
Změnu režimu lze provest smačknutím klavesy `f`.
Následně hráč zadává souřadnice jako v normalním režimu.

### Zadávání souřadnic
Pod hrací plochou je vypsáno, jaké souradnice zvolil hráč.
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
