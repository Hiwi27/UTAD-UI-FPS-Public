# UTAD-UI-FPS-Public
 
-Barra de vida:

    utilizo un timer en tick que depennde de un valor float para asi poder controlar el tiempo del parpadeo al estar con poca vida,
    el porcentaje de vida se establece cuando la clase proyectil hace hit al player y el hud se updatea al ejecutarse el metodo setHealh del character para que vaya acorde con la vida del jugador

-Crosshair:

    para el Crosshair tambien utilizo el tick del widget para poder alargar la animacion de disparo,
    y para cambiar el color si apunta a un enemigo realizo un singleLineTrace por cannal de visibilidad desde el tick del WeaponComponent

-Marcador de daño en pantalla
    
    esta hecho con un timer que se llama desde el metodo onHit del proyectil al llamar al metodo show() del widget
    
-Pantalla Splash

        se ejecuta en el begin play del player y el tiempo de duracion funciona mediante un timer que se inicia en el metod show del widget

-Arbol de abilidades

    Funciona mediante botones los cuales se almacenan en arrays por cada columna y se determina en donde deberia posicionarse con un enumerador, siempre pasara al primer elemento de cada columna al mover de izquierda da derecha y viceversa

