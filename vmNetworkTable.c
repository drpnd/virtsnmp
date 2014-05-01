/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.iterate.conf 19302 2010-08-13 12:19:42Z dts12 $
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "vmNetworkTable.h"
#include "globalHandler.h"

/** Initializes the vmNetworkTable module */
void
init_vmNetworkTable(void)
{
  /* here we initialize all the tables we're planning on supporting */
    initialize_table_vmNetworkTable();
}

/* # Determine the first/last column names */

/** Initialize the vmNetworkTable table by defining its contents and how it's structured */
void
initialize_table_vmNetworkTable(void)
{
    const oid vmNetworkTable_oid[] = {1,3,6,1,2,1,VM_MIB_OID,1,8};
    const size_t vmNetworkTable_oid_len   = OID_LENGTH(vmNetworkTable_oid);
    netsnmp_handler_registration    *reg;
    netsnmp_iterator_info           *iinfo;
    netsnmp_table_registration_info *table_info;

    DEBUGMSGTL(("vmNetworkTable:init", "initializing table vmNetworkTable\n"));

    reg = netsnmp_create_handler_registration(
              "vmNetworkTable",     vmNetworkTable_handler,
              vmNetworkTable_oid, vmNetworkTable_oid_len,
              HANDLER_CAN_RONLY
              );

    table_info = SNMP_MALLOC_TYPEDEF( netsnmp_table_registration_info );
    netsnmp_table_helper_add_indexes(table_info,
                           ASN_INTEGER,  /* index: vmIndex */
                           ASN_INTEGER,  /* index: vmNetworkIndex */
                           0);
    table_info->min_column = COLUMN_VMNETWORKINDEX;
    table_info->max_column = COLUMN_VMNETWORKPHYSADDRESS;
    
    iinfo = SNMP_MALLOC_TYPEDEF( netsnmp_iterator_info );
    iinfo->get_first_data_point = vmNetworkTable_get_first_data_point;
    iinfo->get_next_data_point  = vmNetworkTable_get_next_data_point;
    iinfo->table_reginfo        = table_info;
    
    netsnmp_register_table_iterator( reg, iinfo );

    /* Initialise the contents of the table here */
}

    /* Typical data structure for a row entry */
struct vmNetworkTable_entry {
    /* Index values */
    long vmIndex;
    long vmNetworkIndex;

    /* Column values */

    /* Illustrate using a simple linked list */
    int   valid;
    struct vmNetworkTable_entry *next;
};

struct vmNetworkTable_entry  *vmNetworkTable_head;

/* create a new row in the (unsorted) table */
struct vmNetworkTable_entry *
vmNetworkTable_createEntry(long vmIndex, long  vmNetworkIndex) {
    struct vmNetworkTable_entry *entry;

    entry = SNMP_MALLOC_TYPEDEF(struct vmNetworkTable_entry);
    if (!entry)
        return NULL;

    entry->vmIndex = vmIndex;
    entry->vmNetworkIndex = vmNetworkIndex;
    entry->next = vmNetworkTable_head;
    vmNetworkTable_head = entry;
    return entry;
}
void
vmNetworkTable_createEntryByIndex(long vmIndex, long  vmNetworkIndex)
{
    (void)vmNetworkTable_createEntry(vmIndex, vmNetworkIndex);
}

/* remove a row from the table */
void
vmNetworkTable_removeEntry( struct vmNetworkTable_entry *entry ) {
    struct vmNetworkTable_entry *ptr, *prev;

    if (!entry)
        return;    /* Nothing to remove */

    for ( ptr  = vmNetworkTable_head, prev = NULL;
          ptr != NULL;
          prev = ptr, ptr = ptr->next ) {
        if ( ptr == entry )
            break;
    }
    if ( !ptr )
        return;    /* Can't find it */

    if ( prev == NULL )
        vmNetworkTable_head = ptr->next;
    else
        prev->next = ptr->next;

    SNMP_FREE( entry );   /* XXX - release any other internal resources */
}
void
vmNetworkTable_removeEntryByIndex(long vmIndex, long  vmNetworkIndex)
{
    struct vmNetworkTable_entry *ptr, *prev;

    for ( ptr  = vmNetworkTable_head, prev = NULL;
          ptr != NULL;
          prev = ptr, ptr = ptr->next ) {
        if ( ptr->vmIndex == vmIndex && ptr->vmNetworkIndex == vmNetworkIndex ) {
            break;
        }
    }

    vmNetworkTable_removeEntry(ptr);
}


/* Example iterator hook routines - using 'get_next' to do most of the work */
netsnmp_variable_list *
vmNetworkTable_get_first_data_point(void **my_loop_context,
                                    void **my_data_context,
                                    netsnmp_variable_list *put_index_data,
                                    netsnmp_iterator_info *mydata)
{
    *my_loop_context = vmNetworkTable_head;
    return vmNetworkTable_get_next_data_point(my_loop_context, my_data_context,
                                              put_index_data,  mydata );
}

netsnmp_variable_list *
vmNetworkTable_get_next_data_point(void **my_loop_context,
                                   void **my_data_context,
                                   netsnmp_variable_list *put_index_data,
                                   netsnmp_iterator_info *mydata)
{
    struct vmNetworkTable_entry *entry = (struct vmNetworkTable_entry *)*my_loop_context;
    netsnmp_variable_list *idx = put_index_data;

    if ( entry ) {
        snmp_set_var_typed_integer( idx, ASN_INTEGER, entry->vmIndex );
        idx = idx->next_variable;
        snmp_set_var_typed_integer( idx, ASN_INTEGER, entry->vmNetworkIndex );
        idx = idx->next_variable;
        *my_data_context = (void *)entry;
        *my_loop_context = (void *)entry->next;
        return put_index_data;
    } else {
        return NULL;
    }
}


/** handles requests for the vmNetworkTable table */
int
vmNetworkTable_handler(
    netsnmp_mib_handler               *handler,
    netsnmp_handler_registration      *reginfo,
    netsnmp_agent_request_info        *reqinfo,
    netsnmp_request_info              *requests) {

    netsnmp_request_info       *request;
    netsnmp_table_request_info *table_info;
    struct vmNetworkTable_entry          *table_entry;

    long vmNetworkIfIndex;
    char vmNetworkModel[256];
    char vmNetworkPhysAddress[256];
    int ret;

    DEBUGMSGTL(("vmNetworkTable:handler", "Processing request (%d)\n", reqinfo->mode));

    switch (reqinfo->mode) {
        /*
         * Read-support (also covers GetNext requests)
         */
    case MODE_GET:
        for (request=requests; request; request=request->next) {
            table_entry = (struct vmNetworkTable_entry *)
                              netsnmp_extract_iterator_context(request);
            table_info  =     netsnmp_extract_table_info(      request);
    
            switch (table_info->colnum) {
            case COLUMN_VMNETWORKINDEX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->vmNetworkIndex);
                break;
            case COLUMN_VMNETWORKIFINDEX:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                vmNetworkIfIndex = gh_getVifTable_vmNetworkIfIndex(
                    table_entry->vmIndex, table_entry->vmNetworkIndex);
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            vmNetworkIfIndex);
                break;
            case COLUMN_VMNETWORKMODEL:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                ret = gh_getVifTable_vmNetworkModel(table_entry->vmIndex,
                                                    table_entry->vmNetworkIndex,
                                                    vmNetworkModel,
                                                    sizeof(vmNetworkModel));
                if ( 0 != ret ) {
                    (void)strcpy(vmNetworkModel, "");
                }
                snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                          vmNetworkModel,
                                          strlen(vmNetworkModel));
                break;
            case COLUMN_VMNETWORKPHYSADDRESS:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                ret = gh_getVifTable_vmNetworkPhysAddress(
                    table_entry->vmIndex, table_entry->vmNetworkIndex,
                    vmNetworkPhysAddress, sizeof(vmNetworkPhysAddress));
                if ( 0 != ret ) {
                    (void)strcpy(vmNetworkPhysAddress, "");
                }
                snmp_set_var_typed_value( request->requestvb, ASN_OCTET_STR,
                                          vmNetworkPhysAddress,
                                          strlen(vmNetworkPhysAddress));
                break;
            default:
                netsnmp_set_request_error(reqinfo, request,
                                          SNMP_NOSUCHOBJECT);
                break;
            }
        }
        break;

    }
    return SNMP_ERR_NOERROR;
}