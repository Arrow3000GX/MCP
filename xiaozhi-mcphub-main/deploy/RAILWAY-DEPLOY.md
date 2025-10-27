# Railway Deployment Guide

## Quick Fix for Nixpacks Error

The deployment failed because Railway couldn't automatically detect the build process. Here's how to fix it:

### Solution: Use Dockerfile

Railway now uses the `Dockerfile.railway` which properly configures the build.

### Steps to Deploy

1. **Go to your Railway project**
2. **Click on your service** → Settings
3. **Scroll to "Build & Deploy"**
4. **Change the settings to:**
   - **Build Command**: Leave empty (or `pnpm install && pnpm run build`)
   - **Start Command**: `pnpm start`
   - **Docker Context**: `/` (root)

### Alternative: Let Railway Use Dockerfile

Railway should automatically detect `Dockerfile.railway` and use it.

If it doesn't:
1. Go to Settings → Source
2. Check "Use Docker"
3. Railway will use the Dockerfile

### Environment Variables

Add these in Railway dashboard:

```
DATABASE_URL=your_database_url
NODE_ENV=production
SMART_ROUTING_ENABLED=false
ENABLE_CORS=true
```

### PostgreSQL Database

Railway provides free PostgreSQL:
1. Click "New" → "Database" → "Add PostgreSQL"
2. Copy the DATABASE_URL
3. Add it to your service environment variables

### Manual Deployment via CLI

```bash
# Install Railway CLI
npm install -g @railway/cli

# Login
railway login

# Link your project
railway link

# Deploy
railway up
```

### Common Issues

**Issue**: "Error creating build plan with nixpacks"
**Solution**: Use Dockerfile deployment instead

**Issue**: "Build succeeds but app crashes"
**Solution**: Check that:
- Database is connected
- Environment variables are set
- Port 3000 is exposed

**Issue**: "Slow build times"
**Solution**: Add `.dockerignore` to skip unnecessary files

### Build Command Alternative

If Dockerfile doesn't work, use explicit commands:

**Build Command:**
```bash
pnpm install --frozen-lockfile && pnpm run build
```

**Start Command:**
```bash
pnpm start
```

### Files Created

- `Dockerfile.railway` - Optimized Dockerfile for Railway
- `nixpacks.toml` - Nixpacks configuration (backup option)
- `deploy/railway.json` - Railway project configuration

### Recommended Settings

**Service Settings:**
- **Region**: Closest to you (US East, EU West, etc.)
- **Scaling**: Automatic (Railway scales based on usage)
- **Health Check**: `/api/health`

**Database Settings:**
- **Type**: PostgreSQL
- **Plan**: Free (Hobby) for testing
- **Backups**: Enable if using paid plan

### Monitoring

Railway provides built-in metrics:
- Logs: Real-time application logs
- Metrics: CPU, Memory, Network
- Deployments: Deployment history

### Cost

**Free Tier:**
- $5 credit/month
- ~500 hours of service
- 1GB database storage

**After Free Credits:**
- Pay as you go
- ~$5-10/month for typical usage

