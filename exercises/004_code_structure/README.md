## Proposta

### Código Inicial	

```
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

int main() {
    stdio_init_all();
    if (cyw43_arch_init()) {
        return -1;
    }

    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(500);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(500);
    }
}
```

### Tarefa
Reestruture o código seguindo a arquitetura modular recomendada:
```
    - drivers/: código de controle direto de hardware (ex: LED embutido).
    - hal/: camada de abstração que expõe funções simples e reutilizáveis.
    - app/: lógica da aplicação principal (fluxo de controle, interface com usuário).
    - include/: cabeçalhos necessários.
    - CMakeLists.txt: organize a compilação conforme necessário.
```
	
### Objetivos Específicos
```
    - Crie um driver led_embutido.c que use diretamente a API cyw43_arch.
    - Implemente um hal_led.c com a função hal_led_toggle() que abstraia o driver.
    - Escreva um main.c simples no diretório app/ que apenas use a função da HAL para piscar o LED.
```