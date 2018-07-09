#include "config.h"

#include <rpc/rpc.h>
#include "rquota.h"


bool_t
xdr_getquota_args(xdrs, objp)
	XDR *xdrs;
	getquota_args *objp;
{
	if (!xdr_string(xdrs, &objp->gqa_pathp, RQ_PATHLEN)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->gqa_uid)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_ext_getquota_args(xdrs, objp)
	XDR *xdrs;
	ext_getquota_args *objp;
{
	if (!xdr_string(xdrs, &objp->gqa_pathp, RQ_PATHLEN)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->gqa_type)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->gqa_id)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_rquota(xdrs, objp)
	XDR *xdrs;
	rquota *objp;
{
	if (!xdr_int(xdrs, &objp->rq_bsize)) {
		return (FALSE);
	}
	if (!xdr_bool(xdrs, &objp->rq_active)) {
		return (FALSE);
	}
	if (!xdr_u_int(xdrs, &objp->rq_bhardlimit)) {
		return (FALSE);
	}
	if (!xdr_u_int(xdrs, &objp->rq_bsoftlimit)) {
		return (FALSE);
	}
	if (!xdr_u_int(xdrs, &objp->rq_curblocks)) {
		return (FALSE);
	}
	if (!xdr_u_int(xdrs, &objp->rq_fhardlimit)) {
		return (FALSE);
	}
	if (!xdr_u_int(xdrs, &objp->rq_fsoftlimit)) {
		return (FALSE);
	}
	if (!xdr_u_int(xdrs, &objp->rq_curfiles)) {
		return (FALSE);
	}
	if (!xdr_u_int(xdrs, &objp->rq_btimeleft)) {
		return (FALSE);
	}
	if (!xdr_u_int(xdrs, &objp->rq_ftimeleft)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_gqr_status(xdrs, objp)
	XDR *xdrs;
	gqr_status *objp;
{
	if (!xdr_enum(xdrs, (enum_t *)objp)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_getquota_rslt(xdrs, objp)
	XDR *xdrs;
	getquota_rslt *objp;
{
	if (!xdr_gqr_status(xdrs, &objp->status)) {
		return (FALSE);
	}
	switch (objp->status) {
	case Q_OK:
		if (!xdr_rquota(xdrs, &objp->getquota_rslt_u.gqr_rquota)) {
			return (FALSE);
		}
		break;
	case Q_NOQUOTA:
		break;
	case Q_EPERM:
		break;
	default:
		return (FALSE);
	}
	return (TRUE);
}


