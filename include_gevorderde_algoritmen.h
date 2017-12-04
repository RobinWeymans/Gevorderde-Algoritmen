#pragma once

//Lib
#include "lib\pugixml.hpp"

//Src
#include "src\Efficiente zoekbomen\boom_stats.h"
#include "src\Efficiente zoekbomen\roodzwartboom.h"
#include "src\Efficiente zoekbomen\splayboom.h"
#include "src\Efficiente zoekbomen\zoekboom11.h"
#include "src\Gegevensstructuren voor Strings\trie.h"
#include "src\Gegevensstructuren voor Strings\patriciatrie.h"
#include "src\Gegevensstructuren voor Strings\ternairezoekboom.h"
#include "src\Grafen\bipartietegraaf.h"
#include "src\Grafen\graaf.h"
#include "src\Grafen\graafmetknoopdata.h"
#include "src\Grafen\graafmetknoopentakdata.h"
#include "src\Grafen\graafmettakdata.h"
#include "src\Grafen\kosaraju.h"
#include "src\Grafen\hierholzer.h"
#include "src\Grafen\multigraaf.h"
#include "src\Grafen\bellmanford.h"
#include "src\Indexeren Van Vaste Text\saisclass.h"
#include "src\Meerdimensionale Gegevensstructuren\kdtree.h"
#include "src\Meerdimensionale Gegevensstructuren\quadtree.h"
#include "src\Metaheuristieken\dom.h"
#include "src\Metaheuristieken\dom2.h"
#include "src\Metaheuristieken\probprob.h"
#include "src\Metaheuristieken\simmulanneal11.h"
#include "src\Prioriteitswachtrijen\binomialqueue.h"
#include "src\Prioriteitswachtrijen\leftistheap.h"
#include "src\Prioriteitswachtrijen\pairingheap.h"
#include "src\Prioriteitswachtrijen\skewheap.h"
#include "src\Stroomnetwerken\vergrotendpadzoeker.h"
#include "src\Stroomnetwerken\stroomnetwerk.h"
#include "src\Uitwendige Gegevensstructuren\btree.h"
#include "src\Uitwendige Gegevensstructuren\extendiblehash.h"
#include "src\Uitwendige Gegevensstructuren\hashblok.h"
#include "src\Uitwendige Gegevensstructuren\lineairhash.h"
#include "src\Util\csv.h"
#include "src\Util\pad.h"
#include "src\Util\punt2.h"
#include "src\Util\puntengenerator.h"
#include "src\Util\schijf.h"
#include "src\Util\bitpatroon.h"
#include "src\Zoeken In Strings\knuthmorrispratt.h"
#include "src\Zoeken In Strings\shiftand.h"
#include "src\Random\dynamisch_programmeren.h"

using namespace efficiente_zoekbomen;
using namespace gegevensstructuren_voor_strings;
using namespace grafen;
using namespace meerdimensionale_gegevensstructuren;
using namespace metaheuristieken;
using namespace prioriteitswachtrijen;
using namespace stroomnetwerken;
using namespace uitwendige_gegevensstructuren;
using namespace util;