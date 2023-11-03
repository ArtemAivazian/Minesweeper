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
cmake -Bcmake-build-debug -H. -DCMAKE_BUILD_TYPE=Debug
cmake --build cmake-build-debug
```

Příklady spuštění programu:
```bash
./cmake-build-debug/Minesweeper
```

## Ovládání programu

### Uložení a stahování hry

Pokud hráč už hrál a zmáčknul klavesu `s`, hra už byla uložena do .txt souborů `grid.txt` a `copy.txt`. 
Pokud pak hrač ukončil hru, přičemž hra nebyla vyhrána nebo prohrána, při dálším spůštění programu, hráč bude muset vybrat,
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

### POZOR! 
Jelikož bobmy se generujou náhodně, testy jsou jenom ukázka možných variant
rozvoje hry. Presně zopakovat běh těchto testů není možné.

### Prohra

První spůštění hry:
```
-------------------------------------
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
-------------------------------------
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
-------------------------------------
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
-------------------------------------
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
-------------------------------------
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
-------------------------------------
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
-------------------------------------
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
-------------------------------------
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
-------------------------------------
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
-------------------------------------
Entered coords: 
```

Uživatel zadává souřadnice v normalním režimu: `1 1`
Zadané souřadnice jsou zobrazeny pod aktualizovaným gridem.
Vzhledem k tomu, že buňka s vybranými souřadnicemi byla ve skutečnosti prázdná,
rekurzivně se otevírají všechny možné sousední buňky, dokud se nenajde číslo.
```
-------------------------------------
|   |   |   |   |   |   |   |   |   |
-------------------------------------
| 1 | 1 |   | 1 | 2 | 3 | 3 | 2 | 1 |
-------------------------------------
| 0 | 1 |   | 1 | 0 | 0 | 0 | 0 | 0 |
-------------------------------------
| 0 | 1 |   | 1 | 0 | 0 | 0 | 0 | 0 |
-------------------------------------
| 0 | 1 | 1 | 1 | 0 | 0 | 0 | 0 | 0 |
-------------------------------------
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
-------------------------------------
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
-------------------------------------
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
-------------------------------------
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
-------------------------------------
Entered coords: 1 1
```

Uživatel zadává souřadnice v normalním režimu: `3 1`
Hra je prohraná, protože hráč narazil na bombu.
```
-------------------------------------
|   |   |   |   |   |   |   |   |   |
-------------------------------------
| 1 | 1 |   | 1 | 2 | 3 | 3 | 2 | 1 |
-------------------------------------
| 0 | 1 |   | 1 | 0 | 0 | 0 | 0 | 0 |
-------------------------------------
| 0 | 1 |   | 1 | 0 | 0 | 0 | 0 | 0 |
-------------------------------------
| 0 | 1 | 1 | 1 | 0 | 0 | 0 | 0 | 0 |
-------------------------------------
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
-------------------------------------
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
-------------------------------------
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
-------------------------------------
| 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
-------------------------------------
You lost!
```

### Výhra

Hráč zadává souřadnice: `1 1`
```
-------------------------------------
|   |   | 1 | 0 | 1 |   |   |   |   |
-------------------------------------
|   |   | 1 | 1 | 1 | 1 | 1 | 1 |   |
-------------------------------------
|   |   |   |   | 1 | 0 | 0 | 1 |   |
-------------------------------------
|   |   |   |   | 1 | 0 | 0 | 2 |   |
-------------------------------------
| 1 | 1 | 1 |   | 1 | 2 | 0 | 1 |   |
-------------------------------------
| 0 | 0 | 1 |   |   | 1 | 1 | 1 |   |
-------------------------------------
| 0 | 0 | 2 | 1 |   |   |   |   |   |
-------------------------------------
| 0 | 0 | 0 | 2 |   |   | 1 | 1 | 1 |
-------------------------------------
| 0 | 0 | 0 | 2 |   |   | 1 | 0 | 0 |
-------------------------------------
Entered coords: 1 1
```
Hráč zadává souřadnice: `9 1`
```
-------------------------------------
|   |   | 1 | 0 | 1 |   |   |   |   |
-------------------------------------
|   |   | 1 | 1 | 1 | 1 | 1 | 1 |   |
-------------------------------------
|   |   |   |   | 1 | 0 | 0 | 1 |   |
-------------------------------------
|   |   |   |   | 1 | 0 | 0 | 2 |   |
-------------------------------------
| 1 | 1 | 1 |   | 1 | 2 | 0 | 1 |   |
-------------------------------------
| 0 | 0 | 1 |   |   | 1 | 1 | 1 |   |
-------------------------------------
| 1 | 2 | 2 | 1 |   |   |   |   |   |
-------------------------------------
|   | 2 | 0 | 2 |   |   | 1 | 1 | 1 |
-------------------------------------
|   | 2 | 0 | 2 |   |   | 1 | 0 | 0 |
-------------------------------------
Entered coords: 9 1
```

Hráč přepíná režim na režim prporu zmačknutím tlačitka `f`:
```
-------------------------------------
|   |   | 1 | 0 | 1 |   |   |   |   |
-------------------------------------
|   |   | 1 | 1 | 1 | 1 | 1 | 1 |   |
-------------------------------------
|   |   |   |   | 1 | 0 | 0 | 1 |   |
-------------------------------------
|   |   |   |   | 1 | 0 | 0 | 2 |   |
-------------------------------------
| 1 | 1 | 1 |   | 1 | 2 | 0 | 1 |   |
-------------------------------------
| 0 | 0 | 1 |   |   | 1 | 1 | 1 |   |
-------------------------------------
| 1 | 2 | 2 | 1 |   |   |   |   |   |
-------------------------------------
|   | 2 | 0 | 2 |   |   | 1 | 1 | 1 |
-------------------------------------
|   | 2 | 0 | 2 |   |   | 1 | 0 | 0 |
-------------------------------------
You are in flag mode! Entered coords in flag mode:
```
Označuje proporem určité buňky - zadává souřadnice praporu, dokud nevyhraje:
Hláška `You won!` označuje výhru
```
-------------------------------------
|   |   | 1 | F | 1 |   |   |   |   |
-------------------------------------
|   |   | 1 | 1 | 1 | 1 | 1 | 1 |   |
-------------------------------------
|   |   |   |   | 1 | 0 | F | 1 |   |
-------------------------------------
|   |   |   |   | 1 | F | 0 | 2 |   |
-------------------------------------
| 1 | 1 | 1 |   | 1 | 2 | F | 1 |   |
-------------------------------------
| 1 | F | 1 |   |   | 1 | 1 | 1 |   |
-------------------------------------
| 1 | 2 | 2 | 1 |   |   |   |   |   |
-------------------------------------
|   | 2 | F | 2 |   |   | 1 | 1 | 1 |
-------------------------------------
|   | 2 | F | 2 |   |   | 1 | F | 1 |
-------------------------------------
You won!
```





