..  Copyright 2021

Fallo en los LOGs del ESP 8266
================================================================================

- Revisar el archivo platformio.ini y añadir las siguientes líneas:

    * monitor_speed = 38400
    * monitor_port = /dev/ttyUSB0




1. `Install PlatformIO Core <http://docs.platformio.org/page/core.html>`_
2. Download `development platform with examples <https://github.com/platformio/platform-espressif8266/archive/develop.zip>`_
3. Extract ZIP archive
4. Run these commands:

.. code-block:: bash

    # Change directory to example
    > cd platform-espressif8266/examples/simba-blink

    # Process example project
    > platformio run

    # Upload firmware
    > platformio run --target upload
