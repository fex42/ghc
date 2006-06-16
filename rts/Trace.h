/* -----------------------------------------------------------------------------
 *
 * (c) The GHC Team 2006
 *
 * Debug and performance tracing.  
 *
 * This is a layer over RtsMessages, which provides for generating
 * trace messages with timestamps and thread Ids attached
 * automatically.  Also, multiple classes of messages are supported,
 * which can be enabled separately via RTS flags.
 *
 * All debug trace messages go through here.  Additionally, we
 * generate timestamped trace messages for consumption by profiling
 * tools using this API.
 *
 * ---------------------------------------------------------------------------*/

#ifndef TRACE_H
#define TRACE_H

// -----------------------------------------------------------------------------
// Tracing functions
// -----------------------------------------------------------------------------

void initTracing (void);

// The simple way:
void trace (StgWord32 class, const char *str, ...)
    GNUC3_ATTRIBUTE(format (printf, 2, 3));

// The harder way: sometimes we want to generate a trace message that
// consists of multiple components generated by different functions.
// So we provide the functionality of trace() split into 3 parts: 
//   - traceClass(): a check that the required class is enabled
//   - traceBegin(): print the beginning of the trace message
//   - traceEnd(): complete the trace message (release the lock too).
// 
INLINE_HEADER rtsBool traceClass (StgWord32 class);

void traceBegin (const char *str, ...)
    GNUC3_ATTRIBUTE(format (printf, 1, 2));

void traceEnd (void);

#ifdef DEBUG
#define debugTrace(class, str, ...) trace(class,str, ## __VA_ARGS__)
// variable arg macros are C99, and supported by gcc.
#define debugTraceBegin(str, ...) traceBegin(str, ## __VA_ARGS__)
#define debugTraceEnd() traceEnd()
#else
#define debugTrace(class, str, ...) /* nothing */
#define debugTraceBegin(str, ...) /* nothing */
#define debugTraceEnd() /* nothing */
#endif


// -----------------------------------------------------------------------------
// Message classes, these may be OR-ed together
// -----------------------------------------------------------------------------

// debugging flags, set with +RTS -D<something>
#define DEBUG_sched		   (1<<0)
#define DEBUG_interp		   (1<<1)
#define DEBUG_weak		   (1<<2)
#define DEBUG_gccafs		   (1<<3) 
#define DEBUG_gc		   (1<<4) 
#define DEBUG_block_alloc	   (1<<5) 
#define DEBUG_sanity		   (1<<6) 
#define DEBUG_stable		   (1<<7) 
#define DEBUG_stm   		   (1<<8) 
#define DEBUG_prof		   (1<<9) 
#define DEBUG_gran		   (1<<10)
#define DEBUG_par		   (1<<11)
#define DEBUG_linker		   (1<<12)
#define DEBUG_squeeze              (1<<13)

// PAR debugging flags, set with +RTS -qD<something>
#define PAR_DEBUG_verbose	   (1<<14)
#define PAR_DEBUG_bq		   (1<<15)
#define PAR_DEBUG_schedule         (1<<16)
#define PAR_DEBUG_free       	   (1<<17)
#define PAR_DEBUG_resume     	   (1<<18)
#define PAR_DEBUG_weight     	   (1<<19)
#define PAR_DEBUG_fetch      	   (1<<21)
#define PAR_DEBUG_fish       	   (1<<22)
#define PAR_DEBUG_tables     	   (1<<23)
#define PAR_DEBUG_packet     	   (1<<24)
#define PAR_DEBUG_pack       	   (1<<25)
#define PAR_DEBUG_paranoia         (1<<26)

// GRAN and PAR don't coexist, so we re-use the PAR values for GRAN.
#define GRAN_DEBUG_event_trace     (1<<14)  
#define GRAN_DEBUG_event_stats     (1<<15)
#define GRAN_DEBUG_bq              (1<<16)
#define GRAN_DEBUG_pack            (1<<17)
#define GRAN_DEBUG_checkSparkQ     (1<<18)
#define GRAN_DEBUG_thunkStealing   (1<<19)
#define GRAN_DEBUG_randomSteal     (1<<20)	
#define GRAN_DEBUG_findWork        (1<<21)	
#define GRAN_DEBUG_unused     	   (1<<22)
#define GRAN_DEBUG_pri     	   (1<<23)
#define GRAN_DEBUG_checkLight      (1<<24)	
#define GRAN_DEBUG_sortedQ         (1<<25)	
#define GRAN_DEBUG_blockOnFetch    (1<<26)
#define GRAN_DEBUG_packBuffer      (1<<27)
#define GRAN_DEBUG_BOF_sanity      (1<<28)

// Profiling flags
#define TRACE_sched                (1<<29)


// -----------------------------------------------------------------------------
// PRIVATE below here
// -----------------------------------------------------------------------------

extern StgWord32 classes_enabled;

INLINE_HEADER rtsBool
traceClass (StgWord32 class) { return (classes_enabled & class); }

// -----------------------------------------------------------------------------

#endif /* TRACE_H */
