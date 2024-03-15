/*
*   - La dirección dirección mac station será también el número de serie USB-CDC
*   - Para poder obtener la dirección mac access point WiFi.mode() debe incluir AP
*/

/* TODO definir options1 y options2 */


#include <WiFi.h>
#include <esp_now.h>
#include "serialComms.h"
#include "espnowComms.h"

#define MAX_PEERS       20  // Máximo número de pares a registrar

/* Códigos para el byte options1 para comunicaciones por esp-now */
#define HELLO           24  // código enviado por broadcast por los dispositivos para solicitar emparejamiento al gateway
#define HELLO_RESP      30  // código enviado por el gateway al dispositivo que ha enviado hellow
#define REQUEST_PAIRS   40  // código de envío de los pares registrados al dispositivo remoto
#define SEND_PAIRS      41  // código para solicitar los pares registrados al dispositivo remoto
 

/*
*   mensaje recibido en el callback de recepciónd e esp-now. Los dos primeros bytes son opciones
*   del mensaje, a continuación viene un array de 0 a 248 bytes que contiene el mensaje
*/
struct espnowMsgRaw {
    uint8_t options1;
    uint8_t options2;
    uint8_t data[248];
} receivedRaw, sendRaw;

const uint8_t broadcastAddr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // dejar esta
// const uint8_t broadcastAddr[] = {0xEC, 0xDA, 0x3B, 0x36, 0x45, 0x08};   // MAC del blanco, COM16

uint8_t peerAddresses[MAX_PEERS][6];    // MACs de los pares registrados

uint8_t espnowCh;

// TODO manejar el caso en que se haya producido un error en el envío
void OnDataSent(const uint8_t* mac, esp_now_send_status_t status) 
{
    if (status == ESP_NOW_SEND_SUCCESS) {
        /* envío con éxito */
    }
}

uint8_t* getMac()
{
    return macDir;
}

uint8_t* getApMac()
{
    return macApDir;    
}

/*
*   devuelve el número de pares que tenemos registrados
*/
uint8_t getNumPairs()
{
    esp_now_peer_num_t peers; 
    esp_now_get_peer_num(&peers);
    return peers.total_num;    
}

/* 
*   Envía una consulta broadcast esperando recibir una respuesta desde el gateway. Esta función devolverá la 
*   MAC del gateway. En caso de no obtener respuesta, la MAC devuelta será 0:0:0:0:0:0
*   Esta función no tiene sentido si este dispositivo es gateway
*/
// TODO: gestionar el caso de que haya más de un gateway
void hello()
{   
    sendRaw.options1 = HELLO;
    sendRaw.options2 = 0;
    esp_err_t result = esp_now_send(broadcastAddr, (uint8_t *) &sendRaw, 2);
}

// TODO falta gestionar el caso de que se pierda un par. También qué hacer si se supera el número de pares máximo
void addPeer(const uint8_t * mac, uint8_t chan)
{
    esp_now_peer_info_t peer;
    esp_now_del_peer(mac);      // borramos por si ya estuviera
    esp_now_peer_num_t peers;   // para obtener el número de pares registrados
    esp_now_get_peer_num(&peers);
    if(peers.total_num < MAX_PEERS) {
        memset(&peer, 0, sizeof(esp_now_peer_info_t));
        peer.channel = chan;
        peer.encrypt = false;
        memcpy(peer.peer_addr, mac, 6);
        if (!esp_now_is_peer_exist(mac)) memcpy(peerAddresses[peers.total_num], mac, 6); // añadimos la mac al array local de pares
        esp_err_t status = esp_now_add_peer(&peer);
        if (status != ESP_OK) { /* gestionar error */ }
    }
}

/*
*   envía por ESP-NOW el número de pares registrados y un listado de sus MACs
*/
void sendPairsNow(const uint8_t *peer_addr)
{
    esp_now_peer_num_t peers;   // para obtener el número de pares registrados
    esp_now_get_peer_num(&peers);
    uint8_t peersNum = peers.total_num;
    sendRaw.options1 = SEND_PAIRS;
    sendRaw.options2 = 0;
    sendRaw.data[0] = peersNum; // El primer byte de data será el número total de pares registrados
    memcpy(sendRaw.data, peerAddresses, 6*peersNum); // los siguientes bytes serán las macs 
    esp_err_t result = esp_now_send(peer_addr, (uint8_t *) &sendRaw, 1+6*peersNum);
}

/* 
*   devuelve un array con el número de pares registrados y un listado de sus MACs. Útil para serial
*/
uint8_t *getPairs()
{
    esp_now_peer_num_t peers;   // para obtener el número de pares registrados
    esp_now_get_peer_num(&peers);
    uint8_t peersNum = peers.total_num;
    sendRaw.data[0] = peersNum; // El primer byte de data será el número total de pares registrados
    memcpy((uint8_t *)sendRaw.data+1, peerAddresses, 6*peersNum); // los siguientes bytes serán las macs     
    return sendRaw.data;
}


/*
*   Función callback invocada cuando se recibe algún mensaje por ESP-NOW
*/
void OnDataRecv(const uint8_t* macAddr, const uint8_t* incomingData, int len)
{
    memcpy(&receivedRaw, incomingData, len);    // TODO verificar, supuestamente len se refiere sólo a los datos

    switch (role) {
        case device:
            if (receivedRaw.options1 == HELLO_RESP) {   // respuesta desde gateway a nuestro hello
                memcpy(macGwDir, macAddr, 6);           // Obtenemos la MAC del gateway
                addPeer(macAddr, espnowCh);             // Añadimos el gateway a nuestra lista de pares
                gwLocated = true;
            }
            break;

        case gateway:
            if (receivedRaw.options1 == HELLO) {   // somos gateway y hemos recibido un hello
                addPeer(macAddr, espnowCh);        // Añadimos el device a nuestra lista de pares
                sendRaw.options1 = HELLO_RESP;
                sendRaw.options2 = 0;
                esp_now_send(macAddr, (uint8_t *) &sendRaw, 2); // respondemos para que coja nuestra MAC como gateway
            }
            break;            
    }
}

void configEspnow(const uint8_t espnowCh)
{
    gwLocated = false;  // aun no conocemos la mac el gateway, en caso de ser device
    WiFi.mode(WIFI_STA); 
    
    memcpy(macDir, WiFi.macAddress(macDir), 6);             // dirección station mac de este dispositivo
    memcpy(macApDir, WiFi.softAPmacAddress(macApDir), 6);   // dir Access Point mac de este dispositivo

    esp_err_t result = esp_now_init();
    
    esp_now_register_recv_cb(OnDataRecv);
    esp_now_register_send_cb(OnDataSent);

    addPeer(broadcastAddr, espnowCh);   // añadimos la MAC de broadcast para búsqueda del gateway
}