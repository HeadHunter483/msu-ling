#pragma once

// обозначения морфологических граммем
// (взяты обозначения Yandex MyStem: https://tech.yandex.ru/mystem/doc/grammemes-values-docpage/)

// части речи
#define NOUN	"s"			// существительное
#define VERB	"v"			// глагол
#define ADJ		"a"			// прилагательное
#define ADV		"adv"		// наречие
#define ADVPRO	"advpro"	// местоименное наречие
#define ADJNUM	"anum"		// числительное-прилагательное
#define APRO	"apro"		// местоимение-прилагательное
#define COM		"com"		// часть композита - сложного слова
#define CONJ	"conj"		// союз
#define INTJ	"intj"		// междометие
#define NUM		"num"		// числительное
#define PART	"part"		// частица
#define PRONOUN	"pr"		// предлог
#define NOUNPRO	"spro"		// местоимение-существительное

// времена глаголов
#define PAST	"praet"		// прошедшее
#define PRESENT	"praes"		// настоящее
#define NOTPAST	"inpraes"	// непрошедшее

// падежи
#define IP		"nom"		// именительный
#define RP		"gen"		// родительный
#define DP		"dat"		// дательный
#define VP		"acc"		// винительный
#define TP		"ins"		// творительный
#define PP		"abl"		// предложный
#define RP2		"part"		// партитив (второй родительный)
#define MP		"loc"		// местный (второй предложный)
#define ZP		"voc"		// звательный

// число
#define SINGLE	"sg"		// единственное
#define PLURAL	"pl"		// множественное

// репрезентации и наклонения глагола
#define GERUND	"ger"		// деепричастие
#define PARTIC	"partcp"	// причастие
#define INF		"inf"		// инфинитив
#define INDIC	"indic"		// изъявительное
#define IMPER	"imper"		// повелительное

// формы прилагательных
#define SHORTADJ	"brev"		// краткая
#define FULLADJ	"plen"		// полная
#define POSSADJ	"poss"		// притяжательная

// степень сравнения
#define SUPERDEG	"supr"		// превосходная
#define COMPARDEG	"comp"		// сравнительная

// лица глагола
#define FIRSTP	"1p"		// первое
#define SECONDP	"2p"		// второе
#define THIRDP	"3p"		// третье

// рода
#define MALE	"m"			// мужской
#define FEMALE	"f"			// женский
#define NEUTER	"n"			// средний

// виды глаголов
#define INPERF	"ipf"		// несовершенный
#define PERF	"pf"		// совершенный

// залоги
#define ACTIVE	"act"		// действительный
#define PASSIVE	"pass"		// страдательный

// одушевленность
#define ANIM	"anim"		// одушевленное
#define INANIM	"inan"		// неодушевленное

// переходность глагола
#define TRAN	"tran"		// переходный
#define INTRAN	"intr"		// непереходный

// прочее
#define NONLEX	"nonlex"	// данное слово отсутствует в словаре
#define ABBR	"abr"		// аббревиатура
							// есть еще, но пока не надо