/*

Tuath De
in Norden Engine v0.1a
by Peter Martin
an RPG / RTS with

Philosophy : All new arrays are good if you can afford them(resources).More data about the landscape = more manipulation variables.

	A horde game.Start as a lost Trieme.Open with water Mesh sailing Trireme coming straight in towards Galway from the West.Taking on water.Get to shore fast.Maybe cutscene landing around Galway.
	Land at Galway and help settlers with Aos Si also known as the Sith the the Picts.Because you save them they name you the Tuath De, or (tribe of the gods).
	The Aos Si are magical creatures that live in mounds in the ground.Very dangerous to go in the fairy mounds(the sidhe).Settle Galway after your Trireme shipwrecks.


	Boats and land army system imagine the army marching with a snaking baggage train.
	Encamp to build circle forts that become villages that supply you with weapons, food, and resources depending on their location
	small villages build farm stone walls.Small walls for livestock
	Can play first person when the army is encamped.
	Start as the town of galway.Manage for a while, grow a bit, get resources, then build an army on the islands.
	Galway starts as a small fishing village that you settle as ruler after sailing the seas.
	Open with cutscene of viking style fishing boats sail the settlers into the bay

	Dun Duchathair, Dun Aonghasa, and Dun Eochla come about through missions to establish your military
	Dun Duchathair is first constructed and is constantly occupied throughout the game so grows quite large over time.

	Fighting Mechanics
	Sally forth with a few warriors or ,
	Command Army
	Army consists of :
-Spears
- Two handed Axes & Swords
- Axes & Swords with shields
- Archers


Get a giant in your army after conquoring the giants
A squad of picts
A sqaud of fairies

- Irish landscape
- migratory force
- create settlements
- choose who to leave in the settlement(villagers, archers, axemen).
- small fortresses throughout the landscape with a dozen or so villages who fight with axes and shields.
- fortresses and villages built through encampment.FPS only possible while migration force is encamped.
- FPS RPG has magic, fighting, hunting, fishing
- baggage train snakes behind the Army as it makes it's way through the Irish terrain
- fortify positions with stone walls(circular)
- when you leave your army somewhere for a long time the fortifications grow extensively and outwardly
- good scouts means you can see other armies from a distance
- smaller armies generally move faster however upgrading equipment can assist speed(wagons / boats)
- whereever you encamp usually a small village is set up and some people stay providing you with supplies.
- clear trees for encampment in forest.A little circle is cut in the trees.
- when these little villages are conqured by faries, nature takes over and ruins are left tangled in the trees
- viking style horde fleets
- arrive by boat in the west near galway
- horde army goes around conquoring and taking over territory
- iron mining and smithing in certain towns near mountains
- become king
- forests good for hunting and foraging
- forests cleared as settlements expand.Only miniscule though
- fishing villages and boats
- magic, fairy mounds, and gods
- deep in the forest is dangerous.lots of ambushes / magic
- giants in the northern mountains
- english / welsh in south east, picts in the north east
- fairies(small elf druidic type things) in the forest
- all implemented in the square instance system

- in horde mode operate out of the hero's hall
- in fps play as the hero / king
- upon engangement have option for fps / army overview modes

- Encampment
a circle around camp position is drawn with small bits of stone wall over time
the longer you're there the higher the fortifications get. At a certain size a second stone wall is started

When you click on the old world ladies and try to get them to do something they say something along the lines of
"ohh no I couldn't do that I might break a finger nail" "... get my dress dirty" and so on
The native women are tough and hardy and often good with a bow.
Half giant girl named Yigrid joins you at some point.Good at melee.

QUESTS:
Land at Galway and help settlers with Aos Si also known as the Sith the the Picts
Some men swim to shore to build boats and when you get back the Duns on aran islands are built.
Once Galway is settled some people go back to live on aran islands in a fishing village
Your people call you King
Liberate the Picts from the Giants in the mountains and they'll call you King.
Defeat the Giants and they'll call you King
Liberate the Welsh from the Fairies in the forest and they'll call you King.
Defeat the Picts and they'll call you king
Final Boss fight is against a Roman Legion(1000 strong) that has landed in south perhaps ?
Eject the Romans from your land and it's hapily ever after

NEXT TASK :
Make dragon skulls with stone henge style rocks models.like a druid shrine or something.
implement forest map and render trees on specified tiles
Array of tree classes that feeds the instance VAO in treeMesh.h
Move TreeModel call under forest and try adding forest::forestMap to :
//glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)sizeof(Vertex));


* /