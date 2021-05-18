#encoding "utf8"

NumberW -> AnyWord<wff=/[1-3]?[0-9]?[0-9]?[0-9]?/>;
Prep_1 -> (AnyWord<kwtype=prep>) NumberW;

TempLexMonth -> Prep_1 (Noun<kwtype=month>) (TempLexMonth) (TempLexTime) | Prep Noun<kwtype=month> (TempLexMonth);
TempLexTimeOfDay -> Prep_1 (Noun<kwtype=timeOfDay>) (TempLexTimeOfDay) | Prep Noun<kwtype=timeOfDay> (TempLexTimeOfDay);
TempLexDay -> Prep_1 (Noun<kwtype=day>) (TempLexDay) (TempLexTime) | Prep Noun<kwtype=day> (TempLexDay);
TempLexTime -> Prep_1 (Noun<kwtype=time>) (TempLexTime) (Noun<kwtype=timeOfDay>)  | Prep Noun<kwtype=time> (TempLexTime);

TempLex -> TempLexMonth|TempLexTimeOfDay|TempLexDay|TempLexTime|Adv<kwtype=adv>;

S -> TempLex interp(TempFact.TempLex::not_norm);