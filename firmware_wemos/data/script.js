var gateway = `ws://${window.location.hostname}/ws`;
var websocket;

//Configuración de los sliders táctiles
const sliders = {
    'm1': { track: 'm1-track', thumb: 'm1-thumb', value: 0, touchId: null },
    'm2': { track: 'm2-track', thumb: 'm2-thumb', value: 0, touchId: null }
};

window.addEventListener('load', () => {
    initWebSocket();
    setupTouchControls();
});

function setupTouchControls() {
    Object.keys(sliders).forEach(id => {
        const track = document.getElementById(sliders[id].track);

        track.addEventListener('touchstart', (e) => handleTouch(id, e), { passive: false });
        track.addEventListener('touchmove', (e) => handleTouch(id, e), { passive: false });
        track.addEventListener('touchend', (e) => endTouch(id, e), { passive: false });
        track.addEventListener('touchcancel', (e) => endTouch(id, e), { passive: false });
    });
}

function handleTouch(id, e) {
    e.preventDefault();
    const slider = sliders[id];
    const track = document.getElementById(slider.track);
    const thumb = document.getElementById(slider.thumb);
    const rect = track.getBoundingClientRect();

    // Si ya hay un toque asignado, lo buscamos en los toques actuales.
    // Si no, asignamos el primer toque que entre en este track.
    let touch = null;
    if (slider.touchId !== null) {
        for (let i = 0; i < e.touches.length; i++) {
            if (e.touches[i].identifier === slider.touchId) {
                touch = e.touches[i];
                break;
            }
        }
    } else {
        touch = e.changedTouches[0];
        slider.touchId = touch.identifier;
    }

    if (!touch) return;

    // Calcular posición relativa al track (vertical)
    let y = touch.clientY - rect.top;
    y = Math.max(0, Math.min(y, rect.height));

    // Centrar thumb
    const thumbPos = y - (thumb.offsetHeight / 2);
    thumb.style.top = thumbPos + 'px';

    // Mapear Y (0 a rect.height) a Valor (-255 a 255)
    // 0 arriba (255), rect.height abajo (-255)
    // Usamos una zona muerta en el centro para facilitar el stop
    const mid = rect.height / 2;
    const deadZone = 20;

    let val = 0;
    if (Math.abs(y - mid) > deadZone) {
        val = Math.round(((mid - y) / mid) * 255);
        val = Math.max(-255, Math.min(255, val));
    }

    if (val !== slider.value) {
        slider.value = val;
        sendData(id, val);
    }
}

function endTouch(id, e) {
    const slider = sliders[id];
    let touchReleased = false;

    for (let i = 0; i < e.changedTouches.length; i++) {
        if (e.changedTouches[i].identifier === slider.touchId) {
            touchReleased = true;
            break;
        }
    }

    if (touchReleased) {
        slider.touchId = null;
        const track = document.getElementById(slider.track);
        const thumb = document.getElementById(slider.thumb);

        // Volver al centro
        const mid = track.offsetHeight / 2;
        thumb.style.top = (mid - thumb.offsetHeight / 2) + 'px';

        slider.value = 0;
        sendData(id, 0);
    }
}

function initWebSocket() {
    websocket = new WebSocket(gateway);
    websocket.onopen = () => { document.getElementById('status').innerHTML = 'Conectado'; };
    websocket.onclose = () => {
        document.getElementById('status').innerHTML = 'Reconectando...';
        setTimeout(initWebSocket, 2000);
    };

    websocket.onmessage = function (event) {
        if (event.data.startsWith("dist:")) {
            var valores = event.data.substring(5).split(",");
            if (document.getElementById('distF')) updateSensorUI('distF', valores[0]);
            if (document.getElementById('distB')) updateSensorUI('distB', valores[1]);
        }
    };
}

function updateSensorUI(id, val) {
    let el = document.getElementById(id);
    if (!el) return;
    el.innerHTML = val;
    el.style.color = (parseInt(val) < 150) ? "#ff4444" : "#00ff00";
}

function sendData(id, value) {
    if (websocket && websocket.readyState == WebSocket.OPEN) {
        websocket.send(id + ":" + value);
    }
}

function updateTrim(id, value) {
    const display = document.getElementById('valT' + id.substring(1));
    if (display) display.innerHTML = value;
    sendData(id, value);
}
