# Dexdos
Dexdos
image
Ferramenta para teste de carga de requisições, Dos Security Tool Faça download do script dexdos.c para o seu terminal linux, em seguida compile da seguinte forma:

gcc dexdos.c -o dexdos -lpthread

lpthread: A parte mais importante. Ele vincula a biblioteca de threads do sistema. Sem isso, o compilador dará erro dizendo que pthread_create não foi encontrada.

Como usar: ./dexdos xabaluba.com (ou IP) PORTA THREADS

EX: ./dexdos xabaluba 443 800

Caso não queira digitar toda hora ./dexdos e chamar o script de qualquer lugar faça o seguinte:

mv dexdos /usr/local/bin/dexdos

chmod +x /usr/local/bin/dexdos

Em seguida faça o teste chamando: dexdos

Use com responsabilidade!

Xabaluba
