## Description:

A simple, SKSE based, timed block implementation. If you block withing 0.33 seconds of pressing the block key, your block is successful and it will stagger the attacker + any enemy within a certain range. A timed block will prevent any damage dealt to you.

#### Settings:
- sBlockKey --> set your dual wield block key here if you use a dual wield blocking mod
- fStaggerDistance --> area to stagger enemies around you
- bPerkLockedBlock --> toggle to have the damage prevention locked behind a perk
- PerkModName --> modname of the perk you want to use to lock damage prevention behind. full name including extension e.g. Skyrim.esm
- TimedBlockPerk --> FormID of the perk without load order index and without leading 0. e.g. 0x58f6a

If the format for the mod name or the perk form ID are wrong, the mod will simply skip them but disable damage prevention. enable debug logging to see if your edits were picked up by the dll.

The mod will look up the your assigned block key when you start the game or when you load it and everytime you close the settings menu. If you, for whatever reason, start the game via ``coc locationName`` from the main menu, you need to open the journal menu once and close it so the mod knows your block key.

## Install/Uninstall:
whenever you want, the magic effect from this mod lasts for 0.33 seconds and the perk is empty, so even if it lasts in your game, it won't do anything.

## Requirements:
[Address Library](https://www.nexusmods.com/skyrimspecialedition/mods/32444) for your game version

## Compatibilty:
Does not work with other timed block mods I think. I did not test that.
Works with Skyrim 1.5.97, 1.6.1170 
I only work with 1.6.1170 and that's the version i will troubleshoot so if you find a bug with 1.5, i won't really check it beyond obvious things.

## How is that one different to other timed block mods?
I don't know, I just wanted to have something simple.
All this mod does is prevent damage on timed block and stagger the attacker + close enemies.
With an option for perk mod authors or list authors to put the damage prevention behind a perk. fully optional and by default, it won't lock it behind a perk

## Credits:
- [SeaSparrow](https://next.nexusmods.com/profile/Shekhinaga/about-me?gameId=1704) for the damage hook
- [po3](https://next.nexusmods.com/profile/powerofthree/mods?gameId=1704) for clib-util and specifically the hotkey handling of it
- [colinswrath](https://next.nexusmods.com/profile/colinswrath/about-me?gameId=1704) for blade and blunt and most of the things that are in my Utility.h
- [thirdeyesqueegee](https://next.nexusmods.com/profile/ThirdEye3301/about-me?gameId=1704) for the skse plugin template
- and everyone who worked on SKSE and CommonLib
