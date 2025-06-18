#ifndef SJ_H_
#define SJ_H_
static __inline__ void releaseInfoNode(sdlNODE *pNode)
{
    sdlHEAD *pHead;
    pHead = ((struct sigINFO_NODE *)pNode)->m_owner;
    if ( pHead ) sdlAppendProtect(pHead, pNode);
}

static __inline__ void sigDiscardThreadPending()
{
    sdlNODE *pNode;
    int level;
    sdlHEAD sdl_lst;
/*  Снимем флаг */
    level=intLock();
    threadExecuting->m_sigPending =0;
    sdl_lst = threadExecuting->m_sigInfoPending;
    sdlInitEmpty(&threadExecuting->m_sigInfoPending);
    intUnlock(level);

    while ( (pNode = sdlGetFirst(&sdl_lst)))
    {
/* Вернем сигнал владельцу  */
        releaseInfoNode(pNode);
    }
    return;
}
#endif /*SJ_H_*/
