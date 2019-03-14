#ifndef _GENETICALGORITHM_H
#define _GENETICALGORITHM_H


#include <stdio.h>

//Ⱦɫ��ṹ��
struct Chromosome
{
	int *bit;           //�봮
	int nlen;             //�봮����
	double fitness;       //��Ӧֵ
	double rate_fit;      //��Ե�fitֵ������ռ�ٷֱȣ��������̶��㷨
	double cumu_fit;
	Chromosome()
	{
		bit = NULL;
		fitness = 0.0;
		rate_fit = 0.0;
		cumu_fit = 0.0;
		nlen = 0;
	}
	~Chromosome()
	{
		if (bit)
			delete bit;
	}
	Chromosome(const Chromosome &a) throw()
	{
		fitness = a.fitness;
		rate_fit = a.rate_fit;
		cumu_fit = a.cumu_fit;
		nlen = a.nlen;
		//if (bit)
			//delete bit;
		bit = new int[nlen];
		for ( int i=0 ; i<nlen ; i++ )
		{
			bit[i] = a.bit[i];
		}
	}
	Chromosome& operator= ( const Chromosome& a) throw()
	{
		fitness = a.fitness;
		rate_fit = a.rate_fit;
		cumu_fit = a.cumu_fit;
		nlen = a.nlen;
		if (bit)
			delete bit;
		bit = new int[nlen];
		for ( int i=0 ; i<nlen ; i++ )
		{
			bit[i] = a.bit[i];
		}
		return *this;
	}
	void clone(Chromosome &n) throw()
	{
		n.fitness = fitness;
		n.cumu_fit = cumu_fit;
		n.rate_fit = rate_fit;
		n.nlen = nlen;
		if ( n.bit )
			delete n.bit;
		n.bit = new int[nlen];
		for ( int i=0 ; i<nlen ; i++ )
		{
			n.bit[i] = bit[i];
		}
	}
	void Init(int len) throw()
	{
		nlen = len;
		bit = new int[len];
	}

};

class  CGeneticAlgorithmBase
{
public:
	CGeneticAlgorithmBase(int *poprange,int nrangelen,double dcross_rate=0.9,double dmutate_rate=0.05,int npopulation=500,int niterations=100,int nDNA_size=5,int nTerminateSec = 20 );
	virtual ~CGeneticAlgorithmBase(void);

protected:

	double m_dcross_rate;    //�������
	double m_dmutate_rate;   //����ͻ�����
	int m_npopulation;    //��Ⱥ��С
	int m_niterations;    //��������
	int m_nDNA_size;      //DNA����

	Chromosome *m_spop;           //����

	int *m_poprange;            //�Ӹü����г�ȡ����
	int m_nrangelen;               //���ϳ���

	int m_nTerminateSec;      //��ֹʱ��
	Chromosome m_best;            //���Ÿ���

protected:
	

	virtual bool InitPopulation();         //��������
	virtual bool Objective() = 0;            //������Ӧ��ֵ
	virtual bool Cross();                 //�������
	virtual bool Mutation();              //�������
	virtual bool seletc_prw();            //���̶�ѡ�����
	virtual bool fresh_property();        //������������Ⱥ�ڸ��������ֵ

public:
	virtual bool Generate();          //ִ�й���

	Chromosome getBest() const;       //��ȡ���Ÿ���
private:
	CGeneticAlgorithmBase(){};
	CGeneticAlgorithmBase(const CGeneticAlgorithmBase &a){};
    CGeneticAlgorithmBase& operator = (const CGeneticAlgorithmBase &a){return *this;};

};

#endif