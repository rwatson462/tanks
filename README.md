# Tanks
A little game inspired by the Tanks game on the WII.
Written in c++ using the Pixel Game Engine from One Lone Coder.
Uses assets downloaded from opengameart.org (todo: credit all individuals)

## Aim of the game
Drive your tank around and shoot all the enemies.
(todo: make enemies)
Levels are randomly generated and contain more enemies with more intelligence
as you progress.
There is no "end game", the levels keep regenerating indefinitely.
You have 1 life

## Weapons

1. Tank shell
   - Moves fast
   - Travels in a straight line
   - Does little damage
   - Fast fire rate
2. Armour Piercing tank shell
   - Moves slower than the Tank shell
   - Travels in a straight line
   - Does considerably more damage than the Tank shell
   - Slower fire rate than Tank shell
3. Spread tank shell
   - Fires 3 Tank shells - one dead on target, 1 to the left and 1 to the right
   - Each shell has same stats as the Tank shell
   - Slower fire rate than Armour Piercing tank shell
4. Missile
   - Moves slower than Armour Piercing tank shell
   - Travels in a straight line UNTIL an enemy becomes visible to the missile
   - Homes in on a visible enemy
   - Has a limited turn rate so can be escaped
   - Does a lot of damage
5. Land mine
   - Placed directly beneath the firer
   - Explodes after 5 seconds OR if an enemy or player moves within a specific distance
   - Has a range to the damage done, therefore has the possibility to damage multiple tanks
   - Does the most amount of damage of all weapons (but is the easiest to escape)




### Todo
See TODO.md