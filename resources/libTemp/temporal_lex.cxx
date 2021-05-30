#encoding "utf-8"

Num -> AnyWord<wff=/[1-3]?[0-9]?[0-9]?[0-9]?/>;

Date -> AnyWord<wff=/([0-2]\d|3[01])\.(0\d|1[012])\.(\d{4}|\d{2})/>;

Time -> AnyWord<wff=/(\d|[0-1]\d|2[0-3]):([0-5][0-9])/>;

Part -> Date (Time) | Time (Date) | Num Noun<kwtype=noun> (Num) (Noun<kwtype=noun>);

Period -> Adv<kwtype=adv5> | "каждый" (Num) Noun<kwtype=noun>;

Duration -> Adv<kwtype=adv4> | Prep<kwtype=prep4> Noun<kwtype=noun> | Prep<kwtype=prep4> Part;

End -> Adv<kwtype=adv3> | Prep<kwtype=prep3> Noun<kwtype=noun> | Prep<kwtype=prep3> Part;
End1 -> Prep<kwtype=prep3> Num;

Start -> Adv<kwtype=adv2> | Prep<kwtype=prep2> Noun<kwtype=noun> | Prep<kwtype=prep2> Part;
Start1 -> Prep<kwtype=prep2> Num;

Point -> Prep<kwtype=prep1> Noun<kwtype=noun> | Prep<kwtype=prep1> Part | Num Noun<kwtype=noun> "назад" | Part | Adv<kwtype=adv1>;

Interval -> Start interp(TempLexeme.Start)  (End interp(TempLexeme.End)) (Duration interp(TempLexeme.Duration)) | End interp(TempLexeme.End) (Duration interp(TempLexeme.Duration)) | Duration interp(TempLexeme.Duration) | Start1 interp(TempLexeme.Start) End interp(TempLexeme.End) | Start1 interp(TempLexeme.Start) End1 interp(TempLexeme.End);

TempLex -> Interval interp(TempLexeme.Interval) | Point interp(TempLexeme.Point) | Period interp(TempLexeme.Period);