![GameTitleSmall](https://github.com/SSejuru/SantasToyWarfare/assets/97358860/c443ab9c-6273-491b-a495-8954e0aec13e)

Santa's Toy Warfare is a Multiplayer Christmas FPS where you play with your friends in a snowball fight to get all the toys in the neighborhood!

Each match consists of a 2v2 snowball fight following the rules of a normal "Capture the flag" gamemode, where players must run to the enemy spawn point, capture the gift under the christmas tree, and take it to the team's christmas tree to secure a point! The first team in collecting 4 enemy gifts wins the game.

# Project Features
- First Person Character Movement
- **Action System** (similar to Gameplay Ability System in design)
  - Sprinting Ability 
  - Snowball Projectile Attack
- AttributeComponent (Holding health etc.)
- Heavy use of Events to drive UI and gameplay reactions.
- **GameplayTags** to mark-up Actors, Actions.
- **Multiplayer support** for all features
- Use of Steam Advanced Sessions to create and join player hosted lobbies
- GameMode Logic
  - Team spawning system (Players are divided into teams and are assigned to their own spawn points)
  - Points awarded to teams when capturing a gift
  - Winning and losing conditions

UMG
- Main menu to host/join game
- UI elements for player attributes and projected widgets for allies and enemies health.
- UI elements for game event messages and team scoring
