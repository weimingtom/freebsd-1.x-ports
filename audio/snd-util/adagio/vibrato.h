/*
 * pitch vibrato for gus
 */
struct {
	unsigned char delay;
	unsigned char period;
	unsigned short depth;
} vibrato[128] = {
/* 0	Acoustic Grand */	{  0,  0,   960 },
/* 1	Bright Acoustic */	{  0,  0,   960 },
/* 2	Electric Grand */	{  0,  0,   960 },
/* 3	Honky-tonk Piano */	{  0,  0,   960 },
/* 4	Rhodes Piano */		{  0,  0,   960 },
/* 2	Chorused Piano */	{  0,  0,   960 },
/* 6	Harpsichord */		{  0,  0,   960 },
/* 7	Clavinet */		{  0,  0,   960 },
/* 8	Celesta */		{  0,  0,   960 },
/* 9	Glockenspiel */		{  0,  0,   960 },
/* 10	Musicbox */		{  0,  0,   960 },
/* 11	Vibraphone */		{  0, 16,   960 },
/* 12	Marimba */		{  0,  0,   960 },
/* 13	Xylophone */		{  0,  0,   960 },
/* 14	Tubular Bells */	{  0,  0,   960 },
/* 15	Dulcimer */		{  0,  0,   960 },
/* 16	Hammond Organ */	{  0, 20,   640 },
/* 17	Percussive Organ */	{  0, 20,   640 },
/* 18	Rock Organ */		{  0, 20,   640 },
/* 19	Church Organ */		{  0,  0,   960 },
/* 20	Reed Organ */		{  0, 20,   640 },
/* 21	Accordion */		{  0, 20,   640 },
/* 22	Harmonica */		{ 20, 20,   640 },
/* 23	Tango Accordion */	{  0, 20,   640 },
/* 24	Nylon Guitar */		{ 20, 20,   640 },
/* 25	Steel Guitar */		{  0,  0,   960 },
/* 26	Jazz Guitar */		{ 10, 20,   960 },
/* 27	Clean Guitar */		{  0,  0,   960 },
/* 28	Muted Guitar */		{  0, 20,   960 },
/* 29	Overdriven Guitar */	{ 10, 20,   960 },
/* 30	Distortion Guitar */	{ 10, 20,   960 },
/* 31	Guitar Harmonics */	{ 20, 12,   640 },
/* 32	Acoustic Bass */	{  0,  0,   960 },
/* 33	Finger Bass */		{  0,  0,   960 },
/* 34	Pick Bass */		{  0,  0,   960 },
/* 35	Fretless Bass */	{  0,  0,   960 },
/* 36	Slap Bass 1 */		{  0,  0,   960 },
/* 37	Slap Bass 2 */		{  0,  0,   960 },
/* 38	Synth Bass 1 */		{  0,  0,   960 },
/* 39	Synth Bass 2 */		{  0,  0,   960 },
/* 40	Violin */		{ 10, 26,   640 },
/* 41	Viola */		{ 10, 26,   640 },
/* 42	Cello */		{ 10, 26,   640 },
/* 43	Contrabass */		{ 10, 26,   640 },
/* 44	Tremolo Strings */	{  0, 26,   960 },
/* 45	Pizzicato String */	{  0,  0,   960 },
/* 46	Orchestral Harp */	{  0,  0,   960 },
/* 47	Timpani */		{  0,  0,   960 },
/* 48	String Ensemble 1 */	{  0, 16,   960 },
/* 49	String Ensemble 2 */	{  0, 16,   960 },
/* 50	Synth Strings 1 */	{ 10, 26,   640 },
/* 51	Synth Strings 2 */	{ 10, 26,   640 },
/* 52	Choir Aahs */		{  0, 16,   960 },
/* 53	Voice Oohs */		{  0, 16,   960 },
/* 54	Synth Voice */		{  0, 16,   960 },
/* 55	Orchestra Hit */	{  0,  0,   960 },
/* 56	Trumpet */		{ 10, 20,   640 },
/* 57	Trombone */		{  0,  0,   960 },
/* 58	Tuba */			{  0,  0,   960 },
/* 59	Muted Trumpet */	{ 35, 20,   800 },
/* 60	French Horn */		{  0,  0,   960 },
/* 61	Brass Section */	{  0, 20,   960 },
/* 62	Synth Brass 1 */	{  0, 20,   960 },
/* 63	Synth Brass 2 */	{  0, 20,   960 },

/* 64	Soprano Sax */		{ 35, 26,   640 },
/* 65	Alto Sax */		{ 35, 26,   640 },
/* 66	Tenor Sax */		{ 35, 26,   640 },
/* 67	Baritone Sax */		{ 35, 26,   640 },
/* 68	Oboe */			{ 20, 26,   640 },
/* 69	English Horn */		{ 20, 26,   640 },
/* 70	Bassoon */		{ 20, 26,   640 },
/* 71	Clarinet */		{ 20, 26,   640 },
/* 72	Piccolo */		{  0, 26,   640 },
/* 73	Flute */		{  0, 26,   640 },
/* 74	Recorder */		{ 20, 26,   640 },
/* 75	Pan Flute */		{  0,  0,   960 },
/* 76	Bottle Blow */		{  0,  0,   960 },
/* 77	Shakuhachi */		{  0,  0,   960 },
/* 78	Whistle */		{ 20, 26,   640 },
/* 79	Ocarina */		{  0,  0,   960 },
/* 80	Lead1 squareea */	{  0,  0,   960 },
/* 81	Lead2 sawtooth */	{  0,  0,   960 },
/* 82	Lead3 calliope */	{  0,  0,   960 },
/* 83	Lead4 chiff */		{  0,  0,   960 },
/* 84	Lead5 charang */	{  0, 16,   960 },
/* 85	Lead6 voice */		{  0, 13,   960 },
/* 86	Lead7 fifths */		{  0,  0,   960 },
/* 87	Lead8 brass+ld */	{  0, 26,   640 },
/* 88	Pad1 newage */		{  0, 20,   960 },
/* 89	Pad2 warm */		{  0, 20,   960 },
/* 90	Pad3 polysynth */	{  0, 20,   960 },
/* 91	Pad4 choir */		{  0, 20,   960 },
/* 92	Pad5 bowed */		{  0, 20,   960 },
/* 93	Pad6 metallic */	{  0, 20,   960 },
/* 94	Pad7 halo */		{  0, 20,   960 },
/* 95	Pad8 sweep */		{  0, 20,   960 },
/* 96	FX1 rain */		{  0,  0,   960 },
/* 97	FX2 soundtrack */	{  0,  0,   960 },
/* 98	FX3 crystal */		{  0,  0,   960 },
/* 99	FX4 atmosphere */	{  0,  0,   960 },
/*100	FX5 brightness */	{  0,  0,   960 },
/*101	FX6 goblins */		{  0,  0,   960 },
/*102	FX7 echoes */		{  0,  0,   960 },
/*103	FX8 sci-fi */		{  0,  0,   960 },
/*104	Sitar */		{  0, 40,   640 },
/*105	Banjo */		{  0,  0,   960 },
/*106	Shamisen */		{  0,  0,   960 },
/*107	Koto */			{  0,  0,   960 },
/*108	Kalimba */		{  0,  0,   960 },
/*109	Bagpipe */		{  0,  0,   960 },
/*110	Fiddle */		{  0,  0,   960 },
/*111	Shanai */		{  0,  0,   960 },
/*112	Tinkle Bell */		{  0,  0,   960 },
/*113	Agogo Bells */		{  0,  0,   960 },
/*114	Steel Drums */		{  0,  0,   960 },
/*115	Woodblock */		{  0,  0,   960 },
/*116	Taiko Drum */		{  0,  0,   960 },
/*117	Melodic Tom */		{  0,  0,   960 },
/*118	Synth Drum */		{  0,  0,   960 },
/*119	Reverse Cymbal */	{  0,  0,   960 },
/*120	Guitar Fret Noise */	{  0,  0,   960 },
/*121	Breath Noise */		{  0,  0,   960 },
/*122	Seashore */		{ 40, 80,   320 },
/*123	Bird Tweet */		{  0,  0,   960 },
/*124	Telephone Ring */	{  0,  0,   960 },
/*125	Helicopter Blade */	{  0,  0,   960 },
/*126	Applause/Noise */	{  0,  0,   960 },
/*127	Gunshot */		{  0,  0,   960 }
};