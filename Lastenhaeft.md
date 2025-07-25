# Lastenheft

## Roboterfunktionalitaet

- [ ] kann 3 runden fahren

NACH EINSCHALTEN:
abhaengig von position nachrichten ausgeben.


### States

**AUF START/WAITING:**
- [ ] 5 Hz blinking wenn bereit zu starten. - blink in *waiting start* state
- [ ] zusaetslich folgendes ausgeben in 1 Hz: ` Hey you. I am in state WAITING. Send me
      an ’S’ so that I can start or send me a ’?’ so that I can give you help.`


**NICHT AUF START/SEARCHING:**
- [ ] 1 Hz print: `Not on the starting field, I am in state SEARCHING. Place me
      there please...`
- [ ] LF state anzeigen.

**DRIVING:**
- [ ] Print: `Round and round I go during state DRIVING, currently round
      #$ROUND` in 1 Hz. Runden zaehlen ab 1
- [ ] Print: `A fresh clean start! :-)` when starting to drive (only once after `S`)
- [ ] Print: `YEAH, still in state DRIVING, done round 1, going for round 2/3`
      after finishing round 1
- [ ] Print: `YEAH YEAH, still in state DRIVING, done round 2, going for round 3/3` after finishing round 2
- [ ] Print: `Done. Thank you! I am going into state RESTING now and I will
      reset myself in 5 seconds. Take care of you and me!` after round 3
- [ ] 5 Sekunden reseten, dann von vorne
- [ ] LF - Leuchtdioden state anzeigen. immer

**RESETTING:**
- [ ] Print: `I will be in state RESTING from now on! zzzZZZzzzZZZzzz` einmalig
      wenn entering

**PAUSE:**
- [ ] Lauflicht 10 Hz
- [ ] Print: `I am in state PAUSING .... zzzZZZzzzZZZzzz .... wake me up with P
  again.` with 1 Hz

### Steueranweisungen
- [ ] `S` to start driving - 3 labs default
- [ ] `?` to get help
- [ ] `P` toggle Pause while driving. only in DRIVING

## Print ausgaben
- [ ] Alle Prints mit doppelten Zeilenumbruch
- [ ] Carriage Return muss das letzte zeichen sein.

## Softwarequalitaet

### Coding conventions
- [ ] Space indenting, no tabs (Nope, wont do)
- [ ] only english
- [ ] Sinnvol modularisieren
- [ ] no magic numbers in .c files!!!

### Files
- [ ] files and functions kurz halten
- [ ] eine datei `robot_settings.h` for convenient configuration
- [ ] eine datei `serial_messages.h` for all print messages

### INP/OUTP/PROC
Input / Output / Proccess ist gewuensch, da ich eine State machine baue werde
ich diese Struktur nicht umsetzen.

## Dokumentation
auf english
- [ ] Start page for a rough complete project overview
- [ ] on Start page explain the build proccess
- [ ] Bereitgestellten Sourcecode selbstaendig dokumentieren und auf Conventions
      anpassen
- [ ] doxygen HTML!
- [ ] docu build per makefile
- [ ] Start page image of Robi
- [ ] keinen doku ordner mit abgeben, der wird generiert.
- [ ] doxyfile und Bilder in ordner `res`
- [ ] auf startseite zusatzfunktionen anpreisen!

## Build system
Makefile und nichts anderes!!!
- [ ] `make` ohne zusatz muss builden und flashen
- [ ] relative pfade






