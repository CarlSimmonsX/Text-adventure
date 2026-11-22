#include <iostream>
const int ITEMS = 22;
struct Room {
	std::string desc, exits;
	int item = -1,  interactable = -1, secondItem = -1;
};
Room room[7][7] {
	{
		{"A dimly lit room. The exit south is blocked by the rocks from the cave-in. Looking up, you notice the walls do not reach the cavern's roof\nWrite help to get the instructions","north, east"},
		{"A narrow tunnel slopes upward. Scratches on the stone seem too large to have been made by any human","south,",0},
		{"A forgotten armory. Most weapons have rusted beyond recognition","north, east,  south",8},
		{"A chamber with little space beside the massive statue in its center","north, east, south",-1,1},
		{"A deserted campsite. Whoever stayed here left in a hurry","north, east"},
		{"A dusty room that smells faintly of smoke","north, east, south"},
		{"An abandoned mining site. Rusted tools and collapsed supports lie everywhere","east, south",12}
	},
	{
		{"A rocky corridor. Deep claw marks rake across the walls","east, west"},
		{"A damp room with a shallow stream trickling across the floor","north, east, west",9},
		{"A cave intersection lit by glowing fungi","north, east, south, west"},
		{"A winding passage where every turn looks the same. Strange footprints cover the dusty floor","north, east, south, west"},
		{"A chamber with a low ceiling. You must stoop to move comfortably","east, south, west"},
		{"A room cluttered with debris from countless years of cave-ins.","north, east"},
		{"A small cave with an unnaturally smooth floor as if something large rests here often","south, west"}
	},
	{
		{"A frozen chamber. A thick block of ice catches the light from somewhere above","east, west",1},
		{"A crossroads chamber, filed with human bones. Several tunnels branch off into darkness","north, east, south, west",19},
		{"A circular chamber with peculiar carvings spiraling across the walls.","east, south, west"},
		{"A spacious cavern where every sound echoes loudly","north, east, west"},
		{"A cavern that feels foreboding just being in it. You feel like something is going to happen","east, south, west"}
	},
	{
		{"A storage room packed with rotting crates and barrels","east, north, west",18},
		{"An ancient shrine dominates the room. A gigantic dragon statue stands silently in the center.","east, south, west",2,0},
		{"A cavern adorned with faded banners depicting dragons as kings.","north, east, west",14},
		{"A room dominated by a stone pedestal covered in strange runes.","north, east, south, west",15},
		{"A room containing another dragon statue. Dust blankets everything","north, south, west",-1,2},
		{"A tiny cave with three caves stretching out of it","north, south, west"},
		{"The dragon's lair. A massive room, with scorch marks cover the walls, and charred bones and treasure surround a scorched stone floor","east, south",21}
	},
	{
		{"A cavern littered with broken stalagmites. Cold air drifts through cracks in the walls","east, north, west",5},
		{"A prison-like chamber separated by thick iron bars","east, west, south",4},
		{"A damp cave where water drips steadily from above","north, east, west"},
		{"A twisting tunnel where a cold breeze brushes past your face","north, south, west",-1,5},
		{"A small cave whose walls are covered in soot","north, east, south",20},
		{"A dead-end alcove. The silence here is unsettling","south"},
		{"A corridor, remarkably small for something going out of the dragon's main lair","east, west"}
	},
	{
		{"A quiet grotto. The distant sound of dripping water echoes throughout the cavern.","north, west",3},
		{"An oddly warm cavern. The rock here feels smooth and worn","east, south, west",6},
		{"A chamber containing a strange lever mounted into the stone","east, west",-1,4},
		{"A vaulted chamber where gears protrude from the floor beside another dragon statue","east",17,3},
		{"A chamber whose walls are blackened by ancient fire.","east, west",-1,6},
	   {"A maze entrance. The passages beyond twist endlessly into darkness","north, east"},
		{"A narrow crack in the cavern opens into a hidden passage","south, west"}
	},
	{
		{},
		{"A narrow ledge overlooking a dark chasm. Falling would be fatal","west",7},
		{"A workshop with broken furniture and scattered crafting supplies","north, west",13},
		{"Another lever is attached to the ground in this spacious cavern","north, south, west",-1,7},
		{"A secluded chamber","south, west",10},
		{"A hidden alcove behind a collapsed wall","west",16}
	}
};
struct Item {
	std::string name;
	int status;
	std::string examine, fullName, useTarget, useResult, exitAdded;
};
Item items[ITEMS] {
	{"goblin", -1, "The goblin sneers at you. You realize that he is guarding an entrance north","","","",", north"},
	{"ice", -1, "You faintly see something inside it"," block"},
	{"boulder", -1, "It is  wedged against the statue. Looking at it, it seems like you might be able to turn the statue if you could somehow remove the boulder"},
	{"orc", -1, "You notice it is guarding what looks like a map","","","","east"},
	{"gold",-1, "Your hands can't quite reach the gold", " coins behind metal bars"},
	{"string",0,"An ordinary piece of string"},
	{"candle",0,"At first glance it seems ordinary, but it seems to stay lit no matter what","","ice",    "you melt the ice, revealing a magnet!"},
	{"book",0,"You flip open to a page that says: Once someone tried to give the dragon gold with other metals in it, claiming it was pure. The dragon roasted him alive, but the impure gold is still lying around here somewhere"},
	{"sword",0,"A sturdy blade. Looks like it could do well against ordinary foes, but not a dragon", "", "orc", "You cleave the orc in two"},
	{"water",0,"it doesn't look drinkable"," in a pond"},
	{"bucket",0,"Not very useful without anything in it","","water","You scoop up the water in the bucket"},
	{"torch",-1,"It is firmly mounted to the wall, and it provides a light to the room"},
	{"pickaxe",0,"A solid pickaxe  made of iron, but it looks to dull to mine many rocks"},
	{"whetstone",0,"It looks a little different than most whetstones, like it isn't supposed to be used on swords","","pickaxe","you sharpen the pickaxe"},
	{"journal",0,"It reads: when the dragons see that which seems blank that you cannot, only then will the secret be revealed"},
	{"pedestal",-1,"A towering pedestal in the center of the room, engraved with strange symbols you cannot decipher"},
	{"dragonslayer",0,"A shining blade, capable of cutting through anything!"," sword","dragon"},
	{"gears",-1,"These gears seems like something else might turn them, but you can't manually turn the statue here."},
	{"bones",-1,"A message written near the bones says: \"Two must be on\""},
   {"human",-1,"A message written near the bones says: \"Its right and left must be different\""," bones"},
	{"animal",-1,"A message written near the bones says: \"My right is on\""," bones"},
	{"treasure",0,"There is all kinds of riches here. You could only take some of it though"}
};
Item oldItems[ITEMS];
void saveOriginal() {
    for (int i = 0; i < ITEMS; i++)
        oldItems[i] = items[i];
}
Item newItems[ITEMS] {
	{},
	{"magnet", 0, "good for sticking to anything magnetic","","string", "you tie the magnet to the string"},
	{},
	{"map", 0, "It seems to be blank","","pedestal","you reach up and place the map on the pedestal"},
	{"gold",0, "three 'gold' coins. They do feel lighter than pure gold", " coins","goblin", "The goblin takes your coins with delight and leaves"},
	{"string",1,"A magnet attached to a string"," magnet","gold", "you dangle the magnet through the bars, and the 'gold' is pulled to the magnet. Yeah, that isn't pure gold"},
	{},
	{},
	{},
	{"water",1,"A bucket filled with water"," bucket","torch","You pour the water on the torch, and the flame goes out!"},
	{},
	{},
	{"pickaxe",1,"A pickaxe suitable for mining","","boulder","You break off a piece of the boulder and it rolls away"},
	{},
	{},
	{"map",-1,"The blank map is on the pedestal now. What could happen now?"," on a pedestal"}
};
struct Interactable {
    std::string name;
    int direction;
    int maxDirection;
    int goal;
};
Interactable interactable[8] {
    {"ominous statue of a dragon, gazing to the",1,3,0},
    {"ominous statue of a dragon, gazing to the",2,3,1},
    {"ominous statue of a dragon, gazing to the",0,3,2},
    {"ominous statue of a dragon, gazing to the",2,3,3},
    {"lever",0,1,0},
    {"lever",0,1,0},
    {"lever",0,1,1},
    {"lever",0,1,1}
};
void checkMap(bool lever, std::string successText){
    bool success = true;
    for (int i = 4 * lever; i < 4 + 4 * lever; i++){
        if (interactable[i].direction != interactable[i].goal)
            success = false;
    }
    if (success){
        if (!lever && items[15].name == "map" && items[11].name.empty()) {
            items[15] = {"magic",0,"It tells you the secret of how to get through the maze to the legendary dragonslayer sword","map"};
            std::cout << successText;
        }
        else if (lever && interactable[3].direction == 2){ 
            interactable[3].direction = 3;
            std::cout << successText;
        }
    }
}