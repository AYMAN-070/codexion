*This project has been created as part of the 42 curriculum by ayhammou.*

## Description
Codexion est une simulation de multithreading basée sur le problème du Dîner des Philosophes, adaptée avec des codeurs et des dongles. Le projet met en évidence la gestion des ressources partagées et l'ordonnancement (FIFO et EDF).

## Instructions
- Compilation : `make`
- Exécution : `./codexion [nb_coders] [burnout] [compile] [debug] [refactor] [nb_compiles] [cooldown] [fifo/edf]`
- Exemple : `./codexion 5 800 200 200 10 0 5 fifo`

## Resources
- Documentation officielle de `<pthread.h>`
- Tutoriels sur la synchronisation des threads (Mutex et Variables de Condition).
- Utilisation de l'IA (Gemini) : L'IA a été utilisée comme tuteur pour m'expliquer le fonctionnement des "Condition Variables", le concept de l'Attente Active (Busy Waiting) pour ma fonction `ms_sleep`, et pour m'aider à débugger l'ordonnancement par tas minimum (Min-Heap) pour la file d'attente locale des dongles.

## Blocking cases handled
- **Deadlock :** Évité en instaurant une asymétrie stricte (les pairs prennent le dongle gauche en premier, les impairs prennent le dongle droit en premier).
- **Starvation / Livelock :** Géré grâce à l'implémentation de files d'attente locales par dongle (Min-Heap) avec une gestion des priorités FIFO ou EDF.
- **Délai de refroidissement (Cooldown) :** Géré par un système de micro-siestes (`usleep`) vérifiant périodiquement l'heure de disponibilité sans monopoliser le CPU (évite l'attente active bloquante).

## Thread synchronization mechanisms
- **Mutex (`pthread_mutex_t`) :** Utilisés pour protéger les variables sensibles (état de la simulation, statistiques des codeurs) et verrouiller les dongles lors de leur manipulation.
- **Variables de condition (`pthread_cond_t`) :** Utilisées en tandem avec les mutex pour endormir profondément les threads attendant un dongle (`pthread_cond_wait`) et les réveiller de manière ciblée lorsqu'un dongle est reposé sur la table (`pthread_cond_broadcast`), économisant ainsi les ressources du CPU.