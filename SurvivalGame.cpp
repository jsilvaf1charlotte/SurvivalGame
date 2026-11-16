#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cctype>
#include <time.h>

using namespace std;

class Player {
   public:
   Player(int x, int y){
      health = MAX_HEALTH;
      hunger = MAX_HUNGER;
      thirst = MAX_THIRST;

      this->x = x;
      this->y = y;
      this->score = 0;
   }

   int getScore() const {
           return score;
   }

   void takeDamage(int val){
           this->health -= val;
      if(this->health < 0) this->health = 0;
   }

   void takeTurn(){
           this->thirst--;
      this->hunger--;

      if(this->thirst <= 0){
         this->health--;
         this->thirst = 0;
      }

      if(this->hunger <= 0){
         this->health--;
         this->hunger = 0;
      }

      this->score++;
   }

   string getStats() const {
           stringstream ss;
      ss  << "============\n"
                  << "Health: " << this->health << "\n"
                  << "Hunger: " << this->hunger << "\n"
                  << "Thirst: " << this->thirst << "\n"
                  << "============\n";
      return ss.str();
   }

   bool isAlive() const {
           return this->health > 0;
   }

   void increaseThrist(int val){
           this->thirst = this->thirst + val;
      if(this->thirst > MAX_THIRST) this->thirst = MAX_THIRST;
   }

   void increaseHunger(int val){
           this->hunger += val;
      if(this->hunger > MAX_HUNGER) this->hunger = MAX_HUNGER;
   }

   int x, y;
   private:
   int health, hunger, thirst, score;
   const int MAX_HEALTH = 3;
   const int MAX_HUNGER = 5;
   const int MAX_THIRST = 3;
};

class Land {
   public:
   virtual string getDescription() = 0;
   virtual string visit(Player& player) = 0;
};

class Forest : public Land {
   public:
   string getDescription(){
           return "a densely wooded forest.";
   }

   string visit(Player& player){
           int randomNum = rand() % 100;

      if(randomNum > 74){
         player.takeDamage(1);
         return "You are attacked by a bear while foraging for berries.";
      } else {
         player.increaseHunger(1);
         return "You forage for berries in the woods and eat a few.";
      }
   }
};

class Lake : public Land {
   public:
   string getDescription(){
           return "a clear sparkling lake.";
   }

   string visit(Player& player){
           player.increaseThrist(2);
      return "You visit the lake and drink its clean water";
   }
};

// New land type: a dry desert full of sand dunes
class Desert : public Land {
   public:
   string getDescription(){
           return "an endless stretch of burning sand dunes.";
   }

   string visit(Player& player){
           int randomNum = rand() % 100;

      if(randomNum > 49){
         player.takeDamage(1);
         return "The brutal desert sun beats down on you. Dizzy and sunburned, you stumble through the dunes.";
      } else {
         player.increaseThrist(1);
         return "You spot a lone cactus, crack it open with a rock, and sip the water hidden inside.";
      }
   }
};

// New land type: a steep rocky mountain
class Mountain : public Land {
   public:
   string getDescription(){
           return "a steep granite peak wrapped in fog.";
   }

   string visit(Player& player){
           int randomNum = rand() % 100;

      if(randomNum > 74){
         player.takeDamage(2);
         return "Loose rocks give way under your feet and a rockslide sends you tumbling down the slope.";
      } else {
         player.increaseThrist(1);
         return "You discover an ice-cold spring trickling out of the rocks and drink until you feel human again.";
      }
   }
};

// New land type: a quiet open meadow
class Meadow : public Land {
   public:
   string getDescription(){
           return "a quiet meadow of tall grass and wildflowers.";
   }

   string visit(Player& player){
           int randomNum = rand() % 100;

      if(randomNum > 79){
         player.takeDamage(1);
         return "You step too close to a hidden beehive and sprint away covered in angry bees and stings.";
      } else {
         player.increaseHunger(2);
         return "You dig up wild onions and carrots in the meadow and cook yourself a real meal for once.";
      }
   }
};

const int MAP_SIZE = 10;
Land* map[MAP_SIZE][MAP_SIZE];

void populateMap(){
     for(int i = 0; i < MAP_SIZE; i++){
      for(int j = 0; j < MAP_SIZE; j++){
         // Randomly pick one of the five land types
         int randomNum = rand() % 5;
         switch(randomNum){
case 0: // Forest
               map[i][j] = new Forest;
               break;
case 1: // Lake
               map[i][j] = new Lake;
               break;
case 2: // Desert
               map[i][j] = new Desert;
               break;
case 3: // Mountain
               map[i][j] = new Mountain;
               break;
case 4: // Meadow
               map[i][j] = new Meadow;
               break;
default:
               cout << "Invalid land type selected" << endl;
               break;
         }
      }
     }
}

int main(){
     srand(time(0));

   populateMap();

   Player player(MAP_SIZE/2, MAP_SIZE/2);

   cout << "You wake up face down in the dirt with no phone, no backpack, and no idea where you are." << endl;
   cout << "The only thing you can do now is keep moving and try to survive." << endl;

   while(player.isAlive()){
      // Use modulus so the directions wrap around the edges of the map.
      // Adding MAP_SIZE before the % keeps the result from going negative.
      int north = (player.y - 1 + MAP_SIZE) % MAP_SIZE;
      int south = (player.y + 1) % MAP_SIZE;
      int east  = (player.x + 1) % MAP_SIZE;
      int west  = (player.x - 1 + MAP_SIZE) % MAP_SIZE;

      cout << "To the north you see " << map[player.x][north]->getDescription() << endl;
      cout << "To the east you see " << map[east][player.y]->getDescription() << endl;
      cout << "To the south you see " << map[player.x][south]->getDescription() << endl;
      cout << "To the west you see " << map[west][player.y]->getDescription() << endl;

      cout << "Which way will you go? Enter N, E, S, or W:" << endl;
      char userInput;
      cin >> userInput;

      // If reading input failed, clear the error and ask again
      if(cin.fail()){
         cin.clear();
         cin.ignore(1000, '\n');
         cout << "Invalid input. Please try again." << endl;
         continue;
      }

      // Accept lowercase letters too
      userInput = toupper(userInput);

      switch(userInput){
case 'N':
            player.y = north;
            break;
case 'E':
            player.x = east;
            break;
case 'S':
            player.y = south;
            break;
case 'W':
            player.x = west;
            break;
default:
            // Any other character is invalid: ask again without moving
            cin.ignore(1000, '\n');
            cout << "Invalid direction. Please enter N, E, S, or W." << endl;
            continue;
      }

      cout << map[player.x][player.y]->visit(player) << endl;

      cout << player.getStats() << endl;
      player.takeTurn();
   }

   cout << "The wilderness finally got the best of you. You survived this many turns:" << endl;
   cout << player.getScore() << endl;

   return 0;
}
