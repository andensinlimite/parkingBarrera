#include <Arduino.h>
#include "estados.h"
#include "hardware.h"

Estado estado = INICIAL;
Orden orden = ORDEN_NINGUNA;
Orden orden_boton_ultima = ORDEN_NINGUNA;
long unsigned int cronometro = 0;

bool orden_siguiente(Orden siguiente) {
  orden = siguiente;
}

bool estado_siguiente(Estado siguiente) {
  estado = siguiente;
}

void estado_loop() {

  /* esto es para setear nuevos 'orden' usando boton, siempre que 
     sean diferentes a los de la anterior iteracion */

  Orden orden_boton_nueva;

  if (boton_abrir_manual())
    orden_boton_nueva = ORDEN_ABRIR_MANUAL;
  else if (boton_abrir_automatico())
    orden_boton_nueva = ORDEN_ABRIR_AUTOMATICO;
  else if (boton_cerrar())
    orden_boton_nueva = ORDEN_CERRAR;
  else 
    orden_boton_nueva = ORDEN_NINGUNA;

  if (orden_boton_nueva != orden_boton_ultima) {
    orden_boton_ultima = orden_boton_nueva;
    if (orden_boton_nueva != ORDEN_NINGUNA)
      orden = orden_boton_nueva;
  }
  
  switch (estado) {
    case INICIAL:
      if (final_carrera_cerrada())
        estado_siguiente(CERRADA);
      else if (final_carrera_abierta())
        estado_siguiente(ABIERTA_MANUAL);
      else
        estado_siguiente(ABRIENDO_MANUAL);
      break;
      
    case CERRADA:
      if (orden == ORDEN_ABRIR_AUTOMATICO)
        estado_siguiente(ABRIENDO_AUTOMATICO);
      else if (orden == ORDEN_ABRIR_MANUAL)
        estado_siguiente(ABRIENDO_MANUAL);
      else
        apaga();
      break;
      
    case ABRIENDO_AUTOMATICO:
      if (orden == ORDEN_ABRIR_MANUAL)
        estado_siguiente(ABRIENDO_MANUAL);
      else if (orden == ORDEN_CERRAR)
        estado_siguiente(CERRANDO_MANUAL);
      else if (final_carrera_abierta())
        estado_siguiente(ABIERTA_OCUPADA);
      else
        abre();
      break;
      
    case ABIERTA_OCUPADA:
      if (!obstaculo()) {
        estado_siguiente(ABIERTA_LIBRE);
        cronometro = millis();
      } else if (orden == ORDEN_ABRIR_MANUAL) {
        estado_siguiente(ABIERTA_MANUAL);
      } else if (orden == ORDEN_CERRAR) {
        estado_siguiente(CERRANDO_MANUAL);
      } else {
        apaga();
      }
      break;

    case ABIERTA_LIBRE:
      if (obstaculo())
        estado_siguiente(ABIERTA_OCUPADA);
      else if (orden == ORDEN_ABRIR_MANUAL)
        estado_siguiente(ABIERTA_MANUAL);
      else if (orden == ORDEN_CERRAR)
        estado_siguiente(CERRANDO_MANUAL);
      else if (millis() - cronometro > 5000)
        estado_siguiente(CERRANDO_AUTOMATICO);
      else
        apaga();
      break;

    case CERRANDO_AUTOMATICO:
      if (obstaculo()) 
        estado_siguiente(ABRIENDO_AUTOMATICO);
      else if (orden == ORDEN_ABRIR_MANUAL)
        estado_siguiente(ABRIENDO_MANUAL);
      else if (orden == ORDEN_CERRAR)
        estado_siguiente(CERRANDO_MANUAL);
      else if (final_carrera_cerrada())
        estado_siguiente(CERRADA);
      else
        cierra();
      break;

    case ABRIENDO_MANUAL:
      if (orden == ORDEN_ABRIR_AUTOMATICO) 
        estado_siguiente(ABRIENDO_AUTOMATICO);
      else if (orden == ORDEN_CERRAR) 
        estado_siguiente(CERRANDO_AUTOMATICO);
      else if (final_carrera_abierta())
        estado_siguiente(ABIERTA_MANUAL);
      else
        abre();
      break;
      
    case ABIERTA_MANUAL:
      if (orden == ORDEN_CERRAR) 
        estado_siguiente(CERRANDO_MANUAL);
      else if (orden == ORDEN_ABRIR_AUTOMATICO) 
        estado_siguiente(ABIERTA_OCUPADA);
      else
        apaga();
      break;

    case CERRANDO_MANUAL:
      if (obstaculo())
        estado_siguiente(ABRIENDO_MANUAL);
      else if (orden == ORDEN_ABRIR_MANUAL)
        estado_siguiente(ABRIENDO_MANUAL);
      else if (orden == ORDEN_ABRIR_AUTOMATICO)
        estado_siguiente(ABRIENDO_AUTOMATICO);
      else if (final_carrera_cerrada())
        estado_siguiente(CERRADA);
      else
        cierra();
      break;
  }

  /* marcamos la orden como atendida para no repetirla */
  orden = ORDEN_NINGUNA;
}

const __FlashStringHelper* EstadoStr() {
    switch (estado) {
        case INICIAL:                return F("Inicial");
        case CERRADA:                return F("Cerrada");
        case ABRIENDO_AUTOMATICO:    return F("Abriendo automatico");
        case ABIERTA_OCUPADA:        return F("Abierta ocupada");
        case ABIERTA_LIBRE:          return F("Abierta libre");
        case CERRANDO_AUTOMATICO:    return F("Cerrando automático");
        case ABIERTA_MANUAL:         return F("Abierta manual");
        case ABRIENDO_MANUAL:        return F("Abriendo manual");
        case CERRANDO_MANUAL:        return F("Cerrando manual");
        default:                     return F("[Unknown]");
    }
}

const __FlashStringHelper* OrdenStr() {
    switch (orden) {
        case ORDEN_NINGUNA:          return F("Ninguna");
        case ORDEN_ABRIR_AUTOMATICO: return F("Abrir automatico");
        case ORDEN_ABRIR_MANUAL:     return F("Abrir manual");
        case ORDEN_CERRAR:           return F("Cerrar");
        default:                     return F("[Unknown]");
    }
}

// vim: ai:expandtab:ts=2:sw=2
