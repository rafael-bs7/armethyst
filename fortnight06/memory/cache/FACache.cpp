/* ----------------------------------------------------------------------------

    (EN) armethyst - A simple ARM Simulator written in C++ for Computer Architecture
    teaching purposes. Free software licensed under the MIT License (see license
    below).

    (PT) armethyst - Um simulador ARM simples escrito em C++ para o ensino de
    Arquitetura de Computadores. Software livre licenciado pela MIT License
    (veja a licen�a, em ingl�s, abaixo).

    (EN) MIT LICENSE:

    Copyright 2020 Andr� Vital Sa�de

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

   ----------------------------------------------------------------------------

	INTEGRANTES:
			
	- JULIO CESAR SILVERIO          - 201710891
	- LUIZ FERNANDO ALVES RODRIGUES  -201610292
	- RAFAEL BARBOSA DE SOUZA       - 201820522
	- VINICIUS PONTES PEREIRA LIMA  - 201520766

*/



#include "FACache.h"

#include <cstddef>
#include<math.h>


using namespace std;

/**
 * Constructs a FACache of 'size' bytes organized in lines of 'lineSize' bytes.
 * 
 * The inherited 'associativity' attribute is set to 'size / lineSize' (integer division).
 * 
 * Constraints: 'size' must be a multiple of 'lineSize', and both must be a power of 2.
 */
FACache::FACache(unsigned int size, unsigned int lineSize) : Cache::Cache(size, lineSize, size/lineSize) {

	bool validArgs = false;
	// TODO
	// 1. Note que este construtor chama o construtor da superclasse! Veja o que j� � feito
	//		no construtor da superclasse.
	// 2. Validar os atributos que acabam e ser inicializados na superclasse:
	//		size � um m�ltiplo de lineSize? ou seja, associativity*lineSize == size?
	//		size, lineSize e associativity s�o pot�ncias de 2?

	// validar se associativity*lineSize == size

	if(associativity*lineSize == size){
		verify_size = (size & (size-1));
		verify_lineSize = (lineSize & (lineSize-1));
		verify_associativity = (associativity & (associativity-1));

		if((verify_size==0) and  (verify_lineSize==0) and (verify_associativity==0)){
			validArgs = true;
		}
	}

	if (!validArgs) {
		throw "Bad FACache initialization. Invalid arguments.";
	}
	
	numSets = 1; // fully associative cache does not have sets
	
	// TODO
	// 3. Alocar o atributo 'data' com 'size' bytes
	
//	data = (char *) malloc (sizeof (size));

	data = new char[size];


//	free(pi); libera o char alocado

	// 4. Alocar o atributo 'directory' com 'associativity' elementos uint64_t
	//directory = (uint64_t *) malloc (sizeof (associativity));

	directory = new uint64_t[associativity];
	status = new bool[associativity]=0;
}

FACache::~FACache() {
	// TODO
	// 1. Fazer 'delete' para toda a mem�ria alocada. Uma boa implementa��o s� ter�
	//		alocado mem�ria no construtor.
	
	delete data;
	delete directory;  
	delete status;
}

/**
 * Reads the 32 bit value 'value' in address 'address'.
 * 
 * Returns
 * 		true, if cache hit
 * 		false, if cache miss
 */
bool FACache::read32(uint64_t address, uint32_t * value) {
	// TODO
	// 1. Encontre a tag e o offset do address. O offset � a parte menos significativa
	//		de 'address', correspondente ao n�mero de bits necess�rios para identificar
	//		cada byte na linha. Sugest�o: fa�a um ou mais procedimentos, pois isso ser�
	//		utilizado nos pr�ximos m�todos.
	//			Exemplo: se lineSize = 64 (2^6), s�o 6 bits de offset e 26 bits de tag.
	//			O pr�prio atributo lineSize e sua invers�o bit a bit (~lineSize) podem
	//			ser usados para separar tag e offset do address. Mas ATEN��O: address
	//			� uint64_t e lineSize � unsigned int, fa�a as devidas convers�es!
	// 2. Percorra o diret�rio do cache verificando se a tag � encontrada e, se
	//		encontrada, em qual �ndice foi encontrada. Sugest�o: fa�a um procedimento,
	//		pois isso ser� utilizado nos pr�ximos m�todos.
	// 3. Se a tag n�o foi encontrada (cache miss), retorne false
	// 4. Se a tag foi encontrada (cache hit), leia o valor na linha do cache, escreva-o
	//		no argumento de sa�da 'value' e retorne true.
	//			DESAFIO: toda a cache est� implementada como um �nico vetor de bytes,
	//			declarado na forma do atributo 'data'. A vari�vel 'data' � o endere�o
	//			do primeiro byte da cache, ou seja, � tamb�m o endere�o do in�cio da
	//			primeira linha do cache. O endere�o do �nicio da segunda linha do cache
	//			� 'data + lineSize', o do in�cio da terceira linha do cache � 'data + 2*lineSize',
	//			e assim por diante. Logo, o endere�o do in�cio da linha precisa ser
	//			calculado com o uso do �ndice encontrado no passo 2 acima. Ap�s obtido o
	//			endere�o do in�cio da linha, � preciso convert�-lo para um ponteiro do
	//			tipo de dado sendo lido. N�o � uma tarefa trivial.
	


	int qt_bits_offset = log2(lineSize);
	uint64_t mask = FFFFFFFFFFFFFFFF;   // 64bits
	mask = (mask>>qt_bits_offset)<<qt_bits_offset;

	uint64_t tag = (address &  mask);
	uint32_t offset = (address & ~mask);

	int pos=-1;
	for(int i = 0; i<associativity; i++){
		if(directory[i] == tag){
			pos = i;
			i = associativity;
		}
	}

	if(pos==-1){
		return false;  // tag nao foi encontrada status = miss;
	}else{
		uint32_t* intptr = (uint32_t *)(&(data[lineSize * pos]));
		
		*value = intptr[offset>>2];
		return true;
	}
	
	
	return false;
}

/**
 * Reads the 64 bit value 'value' in address 'address'.
 * 
 * Returns
 * 		true, if cache hit
 * 		false, if cache miss
 */
bool FACache::read64(uint64_t address, uint64_t * value) {
	// TODO
	// A �nica diferen�a da implementa��o de read64 para a de read32 � o Passo 4.
	// 1, 2 e 3. Copie a implementa��o dos Passos 1, 2 e 3 de read32 para este m�todo, pois � o
	//		mesmo c�digo.
	// 4. Repita o Passo 4 de read32, alterando apenas o tipo do dado lido. Em read32
	//		� preciso converter um vetor e o �ndice a ser lido de char * para uint32_t *,
	//		enquanto em read64 � preciso converter de char * para uint64_t *.


	int qt_bits_offset = log2(lineSize);
	uint64_t mask = FFFFFFFFFFFFFFFF;   // 64bits
	mask = (mask>>qt_bits_offset)<<qt_bits_offset;

	uint64_t tag = (address &  mask);
	uint32_t offset = (address & ~mask);

	int pos=-1;
	for(int i = 0; i<associativity; i++){
		if(directory[i] == tag){
			pos = i;
			i = associativity;
		}
	}

	if(pos==-1){
		return false;  // tag nao foi encontrada status = miss;
	}else{
		uint64_t* intptr = (uint64_t *)(&(data[lineSize * pos]));
		*value = intptr[offset>>3];
		return true;
	}
	return false;
}

/**
 * Overwrites the 32 bit value 'value' in address 'address'.
 * 
 * Returns
 * 		true, if cache hit and writing is successful
 * 		false, if cache miss
 */
bool FACache::write32(uint64_t address, uint32_t value) {
	// TODO
	// A diferen�a do write32 para o read32 tamb�m � apenas o Passo 4, que agora se trata de escrita.
	// 1, 2 e 3. Copie a implementa��o dos Passos 1, 2 e 3 de read32 para este m�todo, pois � o
	//		mesmo c�digo.
	// 4. Se a tag foi encontrada (cache hit), escreva o valor do argumento de entrada 'value'
	//		 na linha do cache, altere o status da linha para 'modificado' e retorne true.
	//			DESAFIO 1: o primeiro desafio � o mesmo de read32, a convers�o do tipo do vetor de
	//			dados para a escrita no local correto.
	//			DESAFIO 2: o segundo desafio � o citado status 'modificado'. Ao se alterar um
	//			valor na cache, ele deve ser marcado como modificado, para que o gestor da hierarquia
	//			de mem�ria saiba que precisa atualizar essa linha nos n�veis mais baixos da hierarquia.
	//			Voc� pode criar um vetor de bool, do tamanho do diret�rio do cache, mas � poss�vel
	//			fazer isso no pr�prio diret�rio, usando o bit menos significativo da tag, visto que
	//			esse bit � sempre igual a 0. Voc� pode deixa-lo valendo 0, se a linha n�o possui
	//			modifica��es, e 1, se a linha possui modifica��es. O �nico cuidado agora � no Passo 2,
	//			quando a tag � buscada no diret�rio, mas se voc� implementou o Passo 2 como um
	//			procedimento, ser� f�cil adaptar.



	
	int qt_bits_offset = log2(lineSize);
	uint64_t mask = FFFFFFFFFFFFFFFF;   // 64bits
	mask = (mask>>qt_bits_offset)<<qt_bits_offset;

	uint64_t tag = (address &  mask);
	uint32_t offset = (address & ~mask);

	int pos=-1;
	for(int i = 0; i<associativity; i++){
		if(directory[i] == tag){
			pos = i;
			i = associativity;
		}
	}


	if(pos==-1){
		return false;  // tag nao foi encontrada status = miss;
	}else{
		uint32_t* intptr = (uint32_t* )(&(data[lineSize * pos]));
		intptr[offset>>2] = value;
		status[pos] = true;
		return true;
	}
	return false;
}

/**
 * Overwrites the 64 bit value 'value' in address 'address'.
 * 
 * Returns
 * 		true, if cache hit and writing is successful
 * 		false, if cache miss
 */
bool FACache::write64(uint64_t address, uint64_t value) {
	// TODO
	// A �nica diferen�a da implementa��o de write64 para a de write32 � o Passo 4.
	// 1, 2 e 3. Copie a implementa��o dos Passos 1, 2 e 3 de write32 para este m�todo, pois � o
	//		mesmo c�digo.
	// 4. Repita o Passo 4 de write32, alterando apenas o tipo do dado lido, uint64_t no lugar de
	//		uint32_t.

	int qt_bits_offset = log2(lineSize);
	uint64_t mask = FFFFFFFFFFFFFFFF;   // 64bits
	mask = (mask>>qt_bits_offset)<<qt_bits_offset;

	uint64_t tag = (address &  mask);
	uint32_t offset = (address & ~mask);

	int pos=-1;
	for(int i = 0; i<associativity; i++){
		if(directory[i] == tag){
			pos = i;
			i = associativity;
		}
	}

	if(pos==-1){
		return false;  // tag nao foi encontrada status = miss;
	}else{
		uint64_t* intptr = (uint64_t *)(&(data[lineSize * pos]));
		intptr[offset>>3] = value;
		status[pos] = true;
		return true;
	}
	return false;
}

/**
 * Fetches one line from slower memory and writes to this cache.
 * 
 * The bytes written are the bytes of the line that contains the byte in address
 * 'address'. The total number of bytes copied is exactly 'Cache::lineSize'.
 * 
 * The argument 'data' is a pointer to the whole data of the slower memory from
 * where the data is to be fetched.
 * 
 * Returns:
 * 		NULL, if the line is not set as modified
 * 		a pointer to a copy of the line, if the line is set as modified
 */
char * FACache::fetchLine(uint64_t address, char * data) {
	char *removedLine = NULL;
	int localWriteIndex = writeIndex;
	
	// TODO
	// 1 e 2. Copie a implementa��o dos Passos 1 e 2 de read32 para este m�todo, pois � o
	//		mesmo c�digo.
	// 3. Se a tag foi encontrada no diret�rio, altere a vari�vel localWriteIndex para o �ndice
	//		onde a tag foi encontrada.
	// 4. Verifique se a linha n�mero 'localWriteIndex' est� marcada como modificada. Se n�o
	//		estiver modificada, nada precisa ser feito. Se estiver modificada, fa�a:
	//		1.1 aloque um vetor de 'lineSize' bytes para a vari�vel 'removedLine';
	//		1.2 copie toda a linha n�mero 'localWriteIndex' (utilize memcpy) para 'removedLine'.
	// 5. Copie 'lineSize' bytes de data para a linha n�mero 'localWriteIndex' (utilize memcpy).
	//		ATEN��O: a tag extra�da de 'address' � tamb�m o �ndice do in�cio da �rea de 'data'
	//		que deve ser copiada, ou seja, voc� n�o ir� copiar 'lineSize' bytes a partir do
	//		endere�o 'data', mas sim de &(data[tag]);
	
	// atualiza o �ndice para o pr�ximo fetch (estrat�gia FIFO para escolha da linha)

	
	int qt_bits_offset = log2(lineSize);
	uint64_t mask = FFFFFFFFFFFFFFFF;   // 64bits
	mask = (mask>>qt_bits_offset)<<qt_bits_offset;

	uint64_t tag = (address &  mask);
	uint32_t offset = (address & ~mask);

	int pos=-1;
	for(int i = 0; i<associativity; i++){
		if(directory[i] == tag){
			pos = i;
			i = associativity;
		}
	}

	if(pos!=-1){
		localWriteIndex = pos;
	}

	if(status[localWriteIndex]){
		removedLine = new char [lineSize];
		memcpy(&removedLine, &(data[localWriteIndex*lineSize]), lineSize);
	}

	memcpy(&(data[lineSize*localWriteIndex]), &(data[tag]), lineSize);

	writeIndex = (writeIndex + 1) % associativity;
	return removedLine;
}
