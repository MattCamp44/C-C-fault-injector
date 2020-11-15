cmake_minimum_required(VERSION 3.10) # versione di cmake

project(Prova VERSION 1.0) # nome progetto e versione, questa versione e quella di cmake sopra verranno scritti nell'header di configurazione


add_executable(Prova Prova.cpp) # aggiunge eseguibile alla compilazione

configure_file(ProvaConfig.h.in ProvaConfig.h) # header di configurazione, per ora contiene soltanto la versione

# il file di configurazione verrà scritto nel binary tree, questo aggiunge il path della directory nella lista dei path 
# in cui guardare per includere file
target_include_directories(Prova PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           ) 

#----------------------------------------------------------------------------------------------------------------------------#
# Per fare una compilazione bisogna (from scrach) creare il file Progetto.cpp (.c), creare un file CMakeLists.txt (questo file)
# e metterlo nella cartella dove c'è anche il sorgente, a quel punto si crea ProgettoConfig.h.in che è un header
# di configurazione (per ora contiene soltanto le versioni), l'header ProvaConfig.h viene generato con il primo comando 
# cmake build (sul file cpp bisonga includere quello che sarà ProgettoConfig.h ) a quel punto tramite il comando 
#       
#       cmake build ../boost_cmake_prova
#
# Questa è la build predefinita e in questa fase viene creato il vero header, ora per fare il compiling e il linking  
# e creare l'ELF del file (eseguibili)
# 
#      cmake --build .
# 