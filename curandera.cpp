#include "Curandera.h"

Curandera::Curandera()
    : Aliado("Curandera", 100, 60, 10) {

    arbol = new ArbolHabilidad();

    NodoSkill* base = new NodoSkill("Curación Base", "Cura 30HP a todos", 1);
    NodoSkill* vida = new NodoSkill("Sobrevivencia", "Aumenta vida", 2);
    NodoSkill* masCura = new NodoSkill("Curación Aumentada", "+20HP al curar", 2, 20);
    NodoSkill* menosMana = new NodoSkill("Curación Efectiva", "Reduce maná -5", 2, 0, 0, -5);

    base->izquierda = masCura;
    base->derecha = menosMana;
    masCura->izquierda = vida;

    arbol->setRaiz(base);

    // Cargar animaciones desde spritesheets
    agregarAnimacion("idle",   cargarDesdeSpritesheet("Personajes/Aliados/Curandera/Priest-Idle.png",   100, 100, 6), true);
    agregarAnimacion("attack", cargarDesdeSpritesheet("Personajes/Aliados/Curandera/Priest-Attack.png", 100, 100, 6), false);
    agregarAnimacion("especial",  cargarDesdeSpritesheet("Personajes/Aliados/Curandera/Priest-Heal.png",   100, 100, 6), false);
    agregarAnimacion("hurt",   cargarDesdeSpritesheet("Personajes/Aliados/Curandera/Priest-Hurt.png",   100, 100, 4), false);
    agregarAnimacion("death",  cargarDesdeSpritesheet("Personajes/Aliados/Curandera/Priest-Death.png",  100, 100, 4), false);
    agregarAnimacion("walk",  cargarDesdeSpritesheet("Personajes/Aliados/Curandera/Priest-Walk.png",  100, 100, 6), false);

    setEstado("idle");

    // Cambiar a idle al terminar una animación no cíclica
    connect(this, &Personaje::animacionTerminada, this, [=](QString estado){
        if (estado == "attack" || estado == "especial" || estado=="hurt") {
            setEstado("idle");
        }
    });
}

void Curandera::ataqueEspecial(std::vector<Personaje*>& aliados, std::vector<Personaje*>&) {
    NodoSkill* nodo = arbol->getRaiz();
    int cura = 30;
    int mana = 15;

    if (nodo->desbloqueado) {
        if (nodo->izquierda && nodo->izquierda->desbloqueado)
            cura += nodo->izquierda->curacionExtra;

        if (nodo->derecha && nodo->derecha->desbloqueado)
            mana += nodo->derecha->manaMod;

        if (nodo->izquierda && nodo->izquierda->izquierda && nodo->izquierda->izquierda->desbloqueado)
            vidaMax += 20;
    }

    if (manaActual >= mana) {
        setEstado("curar"); // ← activa la animación visual
        for (auto aliado : aliados) {
            if (aliado->esAliadoPersonaje())
                aliado->restaurarVida(cura);
                //healRuta: (Personajes/Aliados/Curandera/Priest-Heal_Effect,100, 100, 4), false))
        }
        usarMana(mana);
        qDebug() << nombre << " curó a todos por" << cura << "HP.";
    }
}
